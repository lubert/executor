/* Copyright 1986 - 1995 by Abacus Research and
 * Development, Inc.  All rights reserved.
 */

/* Forward declarations in TextEdit.h (DO NOT DELETE THIS LINE) */

#include <base/common.h>
#include <QuickDraw.h>
#include <WindowMgr.h>
#include <ControlMgr.h>
#include <ToolboxUtil.h>
#include <FontMgr.h>
#include <TextEdit.h>
#include <MemoryMgr.h>
#include <ScrapMgr.h>

#include <quickdraw/cquick.h>
#include <mman/mman.h>
#include <textedit/tesave.h>
#include <rsys/hook.h>
#include <algorithm>
#include <textedit/textedit.h>

using namespace Executor;

void Executor::generic_elt_copy(generic_elt_t *dst, generic_elt_t *src)
{
    *dst = *src;
}

void Executor::generic_elt_calc_height_ascent(generic_elt_t *elt)
{
    int16_t savesize, savefont;
    Style saveface;
    FontInfo font_info;

    savesize = PORT_TX_SIZE(qdGlobals().thePort);
    savefont = PORT_TX_FONT(qdGlobals().thePort);
    saveface = PORT_TX_FACE(qdGlobals().thePort);
    TextSize(GENERIC_ELT_SIZE(elt));
    TextFont(GENERIC_ELT_FONT(elt));
    TextFace(GENERIC_ELT_FACE(elt));
    GetFontInfo(&font_info);
    GENERIC_ELT_HEIGHT(elt) = font_info.ascent
                                   + font_info.descent
                                   + font_info.leading;
    GENERIC_ELT_ASCENT(elt) = font_info.ascent;
    TextSize(savesize);
    TextFace(saveface);
    TextFont(savefont);
}

bool Executor::adjust_attrs(TextStyle *orig_attrs, TextStyle *new_attrs,
                            TextStyle *dst_attrs, TextStyle *continuous_attrs,
                            int16_t mode)
{
    GUEST<int16_t> orig_font, orig_size;

    orig_font = TS_FONT(orig_attrs);
    orig_size = TS_SIZE(orig_attrs);

    /* compute the new style for this run */
    if(mode & doFont)
        TS_FONT(dst_attrs) = TS_FONT(new_attrs);
    else
        TS_FONT(dst_attrs) = TS_FONT(orig_attrs);

    /* ### we don't handle doToggle correctly */
    if(mode & doFace)
    {
        if(mode & doToggle && continuous_attrs)
            TS_FACE(dst_attrs) = ((TS_FACE(orig_attrs) & ~TS_FACE(new_attrs))
                                  | (TS_FACE(new_attrs)
                                     ^ TS_FACE(continuous_attrs)));
        else
            TS_FACE(dst_attrs) = TS_FACE(new_attrs);
    }
    else
        TS_FACE(dst_attrs) = TS_FACE(orig_attrs);

    if(mode & addSize)
        TS_SIZE(dst_attrs) = TS_SIZE(new_attrs) + TS_SIZE(orig_attrs);
    else if(mode & doSize)
        TS_SIZE(dst_attrs) = TS_SIZE(new_attrs);
    else
        TS_SIZE(dst_attrs) = TS_SIZE(orig_attrs);

    if(mode & doColor)
        TS_COLOR(dst_attrs) = TS_COLOR(new_attrs);
    else
        TS_COLOR(dst_attrs) = TS_COLOR(orig_attrs);

    return (orig_font != TS_FONT(dst_attrs)
            || orig_size != TS_SIZE(dst_attrs));
}

/* return the index into `runs' that has a starting char `sel'.  if no
   such run exists, create it */

int16_t Executor::make_style_run_at(TEStyleHandle te_style, int16_t sel)
{
    int run_index;

    run_index = te_char_to_run_index(te_style, sel);

    {
        StyleRun *run = TE_STYLE_RUN(te_style, run_index);
        if(STYLE_RUN_START_CHAR(run) == sel)
            return run_index;
    }

    {
        StyleRun *runs;
        int n_runs;
        STHandle style_table;
        STElement *style;
        int style_index;

        /* split the current style into two */
        n_runs = TE_STYLE_N_RUNS(te_style) + 1;
        TE_STYLE_N_RUNS(te_style) = n_runs;
        SetHandleSize((Handle)te_style,
                      TE_STYLE_SIZE_FOR_N_RUNS(n_runs));
        runs = TE_STYLE_RUNS(te_style);
        memmove(&runs[run_index + 2],
                &runs[run_index + 1],
                (n_runs - run_index - 1) * sizeof *runs);

        style_index = STYLE_RUN_STYLE_INDEX(&runs[run_index]);

        /* we created a new run with this style index, update the
       reference count appropriately */
        style_table = TE_STYLE_STYLE_TABLE(te_style);
        style = ST_ELT(style_table, style_index);

        ST_ELT_COUNT(style) = ST_ELT_COUNT(style) + 1;

        STYLE_RUN_START_CHAR(&runs[run_index + 1]) = sel;
        STYLE_RUN_STYLE_INDEX(&runs[run_index + 1]) = style_index;

        return run_index + 1;
    }
}

/* return the style index for the style that has the text attributes
   `attrs'.  if no such style exists, create it.

   assign `count_changed_p' to true if no such style existed, and it
   was necessary to allocate a new style

   if `incr_count_p' this function increases the style reference on
   the returned style by one */

int16_t Executor::get_style_index(TEStyleHandle te_style, TextStyle *attrs, int incr_count_p)
{
    /* these hold the swapped cached height, ascent for the style we are
     searching for */
    GUEST<int16_t> cached_height = -1, cached_ascent = -1;
    int cache_filled_p = false;
    STHandle style_table;
    STElement *st_elt;
    int st_i;
    int n_styles;

    n_styles = TE_STYLE_N_STYLES(te_style);
    style_table = TE_STYLE_STYLE_TABLE(te_style);
    for(st_i = 0; st_i < n_styles; st_i++)
    {
        st_elt = ST_ELT(style_table, st_i);

        if(TS_FONT(attrs) == ST_ELT_FONT(st_elt)
           && TS_SIZE(attrs) == ST_ELT_SIZE(st_elt))
        {
            if(TS_FACE(attrs) == ST_ELT_FACE(st_elt)
               && (TS_COLOR(attrs).red == ST_ELT_COLOR(st_elt).red
                   && TS_COLOR(attrs).green == ST_ELT_COLOR(st_elt).green
                   && TS_COLOR(attrs).blue == ST_ELT_COLOR(st_elt).blue))
            {
                if(incr_count_p)
                    ST_ELT_COUNT(st_elt) = ST_ELT_COUNT(st_elt) + 1;
                return st_i;
            }
            else if(!cache_filled_p)
            {
                cached_height = ST_ELT_HEIGHT(st_elt);
                cached_ascent = ST_ELT_ASCENT(st_elt);
                cache_filled_p = true;
            }
        }
    }

    /* a style not already in the style table was asked for.  create it */
    n_styles++;
    TE_STYLE_N_STYLES(te_style) = n_styles;
    SetHandleSize((Handle)style_table,
                  STYLE_TABLE_SIZE_FOR_N_STYLES(n_styles));
    st_elt = ST_ELT(style_table, n_styles - 1);

    ST_ELT_COUNT(st_elt) = incr_count_p ? 1 : 0;

    ST_ELT_FONT(st_elt) = TS_FONT(attrs);
    ST_ELT_FACE(st_elt) = TS_FACE(attrs);
    ST_ELT_SIZE(st_elt) = TS_SIZE(attrs);
    ST_ELT_COLOR(st_elt) = TS_COLOR(attrs);

    if(cache_filled_p)
    {
        ST_ELT_HEIGHT(st_elt) = cached_height;
        ST_ELT_ASCENT(st_elt) = cached_ascent;
    }
    else
        generic_elt_calc_height_ascent(ST_ELT_TO_GENERIC_ELT(st_elt));

    return n_styles - 1;
}

/* decrease the reference count of style at `style_index' by one */

void Executor::release_style_index(TEStyleHandle te_style, int16_t style_index)
{
    STHandle style_table;
    STElement *st_elt;

    style_table = TE_STYLE_STYLE_TABLE(te_style);
    st_elt = ST_ELT(style_table, style_index);
    gui_assert(ST_ELT_COUNT(st_elt) > 0);
    ST_ELT_COUNT(st_elt) = ST_ELT_COUNT(st_elt) - 1;
}

/* `release_style_index ()' only decreases the reference count, so
   there may some styles with zero reference counts.  remove those
   styles from the style table, and update the style count
   appropriately */

void Executor::stabilize_style_info(TEStyleHandle te_style)
{
    STHandle style_table;
    STElement *st_elt;
    /* map from original style indexes to new style indexes */
    GUEST<int16_t> *index_map;
    int16_t n_styles, n_runs;
    int i;

    n_styles = TE_STYLE_N_STYLES(te_style);
    style_table = TE_STYLE_STYLE_TABLE(te_style);
    index_map = (GUEST<int16_t> *)alloca(n_styles * sizeof *index_map);

    for(i = 0; i < n_styles; i++)
        index_map[i] = i;

    for(i = 0; i < n_styles; i++)
    {
        st_elt = ST_ELT(style_table, i);

        if(!ST_ELT_COUNT(st_elt))
        {
            /* make sure that the last style element in the table is
	     used */
            for(;;)
            {
                STElement *last_st_elt;

                last_st_elt = ST_ELT(style_table, n_styles - 1);
                if(ST_ELT_COUNT(last_st_elt))
                    break;

                n_styles--;

                if(last_st_elt == st_elt)
                    goto done;
            }

            /* there are no runs with the style index `i', so give that
	     index to the last style in the style table, and shrink
	     the style table by 1 */
            *ST_ELT(style_table, i) = *ST_ELT(style_table, n_styles - 1);
            index_map[n_styles - 1] = i;

            /* so that we can verify, when we change the run style
	     indexes, that noone refers to this map index */
            index_map[i] = -1;

            n_styles--;
        }
    }
done:

    TE_STYLE_N_STYLES(te_style) = n_styles;
    SetHandleSize((Handle)style_table,
                  STYLE_TABLE_SIZE_FOR_N_STYLES(n_styles));

    n_runs = TE_STYLE_N_RUNS(te_style);

    /* now map over every run, and map the orig style index to
     it's new style index */
    for(i = 0; i < n_runs; i++)
    {
        StyleRun *run;

        run = TE_STYLE_RUN(te_style, i);
        STYLE_RUN_STYLE_INDEX(run)
            = index_map[STYLE_RUN_STYLE_INDEX(run)];
        gui_assert(STYLE_RUN_STYLE_INDEX(run) != -1);
    }
}

static void
combine_run_with_next(TEStyleHandle te_style, int16_t run_index)
{
    StyleRun *runs;
    int16_t n_runs;
    STHandle style_table;
    STElement *style;
    int16_t style_index;

    n_runs = TE_STYLE_N_RUNS(te_style);
    runs = TE_STYLE_RUNS(te_style);

    memmove(&runs[run_index + 1], &runs[run_index + 2],
            (n_runs - run_index - 1) * sizeof *runs);

    n_runs--;
    TE_STYLE_N_RUNS(te_style) = n_runs;
    SetHandleSize((Handle)te_style,
                  TE_STYLE_SIZE_FOR_N_RUNS(n_runs));

    style_index = STYLE_RUN_STYLE_INDEX(&runs[run_index]);
    style_table = TE_STYLE_STYLE_TABLE(te_style);
    style = ST_ELT(style_table, style_index);
    ST_ELT_COUNT(style) = ST_ELT_COUNT(style) - 1;
}

void Executor::te_style_combine_runs(TEStyleHandle te_style)
{
    int16_t current_run_index, n_runs;

    /* remove any extra run fragmentation; if two adjacent runs are the
     same style, combine them */
    n_runs = TE_STYLE_N_RUNS(te_style);
    for(current_run_index = 0;
        current_run_index < n_runs - 1;)
    {
        StyleRun *current_run, *next_run;

        current_run = TE_STYLE_RUN(te_style, current_run_index);
        next_run = TE_STYLE_RUN(te_style, current_run_index + 1);

        if(STYLE_RUN_STYLE_INDEX(current_run) == STYLE_RUN_STYLE_INDEX(next_run))
        {
            /* styles are the same, combine them */
            combine_run_with_next(te_style, current_run_index);

            n_runs--;
        }
        else
            current_run_index++;
    }
}

/* add the attributes specified by `mode' in `attrs' to the range of
   text between `start' and `end' in the text edit record `te'.  note
   mode may also include `addSize' */

static void
te_add_attrs_to_range(TEHandle te,
                      int16_t start, int16_t end,
                      TextStyle *attrs, int16_t mode)
{
    TEStyleHandle te_style;
    STHandle style_table;
    int16_t start_run_index, end_run_index;
    int16_t current_run_index;

    TextStyle continuous_attrs;

    if(mode & doToggle)
    {
        GUEST<int16_t> continuous_mode = doFace;

        TS_FACE(&continuous_attrs) = TS_FACE(attrs);
        TEContinuousStyle(&continuous_mode, &continuous_attrs, te);
    }

    te_style = TE_GET_STYLE(te);
    style_table = TE_STYLE_STYLE_TABLE(te_style);

    /* make sure that a style run starts and `start' and that a style
     run starts at `end' */
    start_run_index = make_style_run_at(te_style, start);
    end_run_index = make_style_run_at(te_style, end);

    {
        HLockGuard guard(te_style);

        StyleRun *runs = TE_STYLE_RUNS(te_style);

        /* now go through each style and make the appropriate change */
        for(current_run_index = start_run_index;
            current_run_index < end_run_index;
            current_run_index++)
        {
            StyleRun *current_run;
            TextStyle new_attrs;
            STElement *orig_style;
            int16_t orig_style_index;
            int16_t new_style_index;

            current_run = &runs[current_run_index];
            orig_style_index = STYLE_RUN_STYLE_INDEX(current_run);
            orig_style = ST_ELT(style_table, orig_style_index);

            adjust_attrs(ST_ELT_TO_ATTR(orig_style), attrs, &new_attrs,
                         &continuous_attrs, mode);

            /* `current_run' valid over call to `get_style_index ()'
	      since `te_style' is locked */
            new_style_index = get_style_index(te_style, &new_attrs, true);
            release_style_index(te_style, orig_style_index);

            STYLE_RUN_STYLE_INDEX(current_run) = new_style_index;
        }
    }

    stabilize_style_info(te_style);
    te_style_combine_runs(te_style);
}

TEHandle Executor::C_TEStyleNew(const Rect *dst, const Rect *view)
{
    FontInfo font_info;
    int16_t font_height;
    TEHandle teh;
    TEStyleHandle te_style;
    StScrpHandle stsh;
    ScrpSTElement *stp;
    LHHandle lh_table;
    LHPtr lh;
    NullSTHandle tempnullsth;
    STHandle style_table;

    teh = TENew(dst, view);

    HASSIGN_3(teh,
              lineHeight, -1,
              fontAscent, -1,
              txSize, -1);

    te_style = (TEStyleHandle)NewHandle(TE_STYLE_SIZE_FOR_N_RUNS(1));

    (*te_style)->nRuns = 1;
    (*te_style)->runs[0].startChar = 0;
    (*te_style)->runs[0].styleIndex = 0;
    (*te_style)->runs[1].startChar = 1;
    (*te_style)->runs[1].styleIndex = -1;

    (*te_style)->nStyles = 1;

    /* font info used to fill in the fisrt style element */
    GetFontInfo(&font_info);
    font_height = (font_info.ascent
                   + font_info.descent
                   + font_info.leading);
    style_table = (STHandle)NewHandle(sizeof(STElement));
    HASSIGN_7(style_table,
              stCount, 1,
              stFont, PORT_TX_FONT(qdGlobals().thePort),
              stFace, PORT_TX_FACE(qdGlobals().thePort),
              stSize, PORT_TX_SIZE(qdGlobals().thePort),
              stColor, ROMlib_black_rgb_color,
              stHeight, font_height,
              stAscent, font_info.ascent);

    TE_STYLE_STYLE_TABLE(te_style) = style_table;

    lh_table = (LHHandle)NewHandle(sizeof(LHElement));
    lh = *lh_table;
    LH_HEIGHT(lh) = font_height;
    LH_ASCENT(lh) = font_info.ascent;

    TE_STYLE_LH_TABLE(te_style) = lh_table;

    (*te_style)->teRefCon = 0;

    tempnullsth = (NullSTHandle)NewHandle(sizeof(NullSTRec));
    (*te_style)->nullStyle = tempnullsth;
    stsh = (StScrpHandle)NewHandle(sizeof(StScrpRec));
    (*tempnullsth)->nullScrap = (StScrpHandle)stsh;
    (*tempnullsth)->TEReserved = 0;
    (*stsh)->scrpNStyles = 0;

    stp = (*stsh)->scrpStyleTab;
    stp->scrpFont = PORT_TX_FONT(qdGlobals().thePort);
    stp->scrpFace = PORT_TX_FACE(qdGlobals().thePort);
    stp->scrpSize = PORT_TX_SIZE(qdGlobals().thePort);
    stp->scrpColor.red = 0; /* black ? */
    stp->scrpColor.green = 0; /* black ? */
    stp->scrpColor.blue = 0; /* black ? */
    stp->scrpStartChar = 0;

    stp->scrpHeight = font_height;
    stp->scrpAscent = font_info.ascent;

    TESetStyleHandle(te_style, teh);

    TE_SLAM(teh);

    return teh;
}

void Executor::C_TESetStyleHandle(TEStyleHandle theHandle, TEHandle teh)
{
    if(!TE_STYLIZED_P(teh))
        return;
    *(GUEST<TEStyleHandle> *)&(*teh)->txFont = theHandle;
}

TEStyleHandle Executor::C_TEGetStyleHandle(TEHandle teh)
{
    return TE_GET_STYLE(teh);
}

StScrpHandle Executor::C_TEGetStyleScrapHandle(TEHandle te)
{
    StScrpHandle scrap;
    StyleRun *runs;
    TEStyleHandle te_style;
    STHandle style_table;
    int16_t start, end, length;
    int16_t start_run_index, end_run_index;
    int16_t scrap_n_styles;
    int i;

    TE_SLAM(te);

    start = TE_SEL_START(te);
    end = TE_SEL_END(te);
    length = TE_LENGTH(te);
    if(!TE_STYLIZED_P(te))
        return nullptr;

    if(end < start)
    {
        warning_unexpected("end < start");
        end = start;
    }

    te_style = TE_GET_STYLE(te);
    style_table = TE_STYLE_STYLE_TABLE(te_style);
    if(end > length)
        end = length;

    start_run_index = make_style_run_at(te_style, start);
    end_run_index = make_style_run_at(te_style, end);

    scrap_n_styles = std::max(end_run_index - start_run_index, 1);
    scrap = (StScrpHandle)NewHandle(SCRAP_SIZE_FOR_N_STYLES(scrap_n_styles));
    SCRAP_N_STYLES(scrap) = scrap_n_styles;

    if(start == end)
        warning_unimplemented("should check null scrap, first");

    runs = TE_STYLE_RUNS(te_style);

    for(i = 0; i < scrap_n_styles; i++)
    {
        ScrpSTElement *scrap_elt;
        StyleRun *current_run;
        STElement *style;

        current_run = &runs[start_run_index + i];
        style = ST_ELT(style_table, STYLE_RUN_STYLE_INDEX(current_run));
        scrap_elt = SCRAP_ST_ELT(scrap, i);

        generic_elt_copy(SCRAP_ELT_TO_GENERIC_ELT(scrap_elt),
                         ST_ELT_TO_GENERIC_ELT(style));
        SCRAP_ELT_START_CHAR(scrap_elt)
            = STYLE_RUN_START_CHAR(current_run) - start;
    }
    te_style_combine_runs(te_style);

    return scrap;
}

void Executor::C_TEStyleInsert(Ptr text, LONGINT length, StScrpHandle hST,
                              TEHandle te)
{
    TE_SLAM(te);
    ROMlib_tedoitall(te, text, length, true, hST);
    TE_SLAM(te);
}

INTEGER Executor::C_TEGetOffset(Point pt, TEHandle te)
{
    int16_t retval;
    GUEST<Point> sp;

    sp = TE_SEL_POINT(te);
    TE_SEL_POINT(te).h = pt.h;
    TE_SEL_POINT(te).v = pt.v;
    retval = TE_DO_TEXT(te, 0, TE_LENGTH(te), teFind);
    TE_SEL_POINT(te) = sp;

    return retval;
}

LONGINT Executor::C_TEGetPoint(INTEGER offset, TEHandle teh)
{
    Point p;
    int ascent;

    TE_CHAR_TO_POINT(teh, offset, &p);
    HLockGuard guard(teh);
    TEPtr tep = *teh;
    int lineno;

    lineno = TEP_CHAR_TO_LINENO(tep, offset);
    ascent = TEP_ASCENT_FOR_LINE(tep, lineno);

    return ((int32_t)(p.v + ascent) << 16) + (int32_t)p.h;
}

int32_t Executor::C_TEGetHeight(LONGINT endLine, LONGINT startLine,
                                TEHandle teh)
{
    int32_t retval;

    if(startLine > 0)
        startLine--;
    else
        startLine = 0;

    endLine = std::min<LONGINT>(TE_N_LINES(teh), endLine);
    if(endLine < 0)
        endLine = 0;
    else if(endLine > 0)
        endLine--;

    /* ### 1. write a `swap' macro, that should go into `rsys/macros.h',
     and 2. pin start and end by `TE_N_LINES ()' */
    if(startLine > endLine)
    {
        uint32_t temp;

        temp = startLine;
        startLine = endLine;
        endLine = temp;
    }

    if(TE_STYLIZED_P(teh))
    {
        TEStyleHandle te_style;
        LHElement *l, *le;

        retval = 0;

        te_style = TE_GET_STYLE(teh);

        l = *(*te_style)->lhTab + startLine;
        le = l + endLine - startLine;
        for(; l <= le; l++)
            retval += l->lhHeight;
    }
    else
        retval = TE_LINE_HEIGHT(teh) * (endLine - startLine + 1);

    return retval;
}

void Executor::C_TEGetStyle(int16_t sel, TextStyle *attrs,
                            GUEST<int16_t> *line_height,
                            GUEST<int16_t> *font_ascent, TEHandle te)
{
    if(TE_STYLIZED_P(te))
    {
        TEStyleHandle te_style;
        STElement *style;
        StyleRun *runs;
        int run_index;

        te_style = TE_GET_STYLE(te);
        runs = TE_STYLE_RUNS(te_style);
        run_index = te_char_to_run_index(te_style, sel);
        style = ST_ELT(TE_STYLE_STYLE_TABLE(te_style),
                       STYLE_RUN_STYLE_INDEX(&runs[run_index]));

        TS_FONT(attrs) = ST_ELT_FONT(style);
        TS_FACE(attrs) = ST_ELT_FACE(style);
        TS_SIZE(attrs) = ST_ELT_SIZE(style);
        TS_COLOR(attrs) = ST_ELT_COLOR(style);

        *line_height = ST_ELT_HEIGHT(style);
        *font_ascent = ST_ELT_ASCENT(style);
    }
    else
    {
        TS_FONT(attrs) = TE_TX_FONT(te);
        TS_FACE(attrs) = TE_TX_FACE(te);
        TS_SIZE(attrs) = TE_TX_SIZE(te);
        TS_COLOR(attrs) = ROMlib_black_rgb_color;
        *line_height = TE_LINE_HEIGHT(te);
        *font_ascent = TE_FONT_ASCENT(te);
    }
}

void Executor::C_TEStylePaste(TEHandle te)
{
    Handle hText;
    GUEST<int32_t> dummy;
    /* length of the scrap text */
    int16_t length, retval;
    StScrpHandle scrap;

    hText = NewHandle(1);
    length = GetScrap(hText, "TEXT"_4, &dummy);
    if(length < 0)
    {
        /* error, there is no scrap element */
        DisposeHandle(hText);

        /* remove the selected text */
        ROMlib_tedoitall(te, nullptr, 0, false, nullptr);
        return;
    }

    scrap = (StScrpHandle)NewHandle(sizeof(StScrpRec));
    retval = GetScrap((Handle)scrap, "styl"_4, &dummy);
    if(retval < 0)
    {
        DisposeHandle((Handle)scrap);
        scrap = nullptr;
    }

    {
        HLockGuard guard(hText);

        ROMlib_tedoitall(te, *hText, length, false, scrap);
    }
    if(scrap)
        DisposeHandle((Handle)scrap);
    DisposeHandle(hText);
}

static void
te_do_redraw(TEHandle te)
{
    int16_t cal_start, cal_end, sel_start, sel_end;

    TESAVE(te);

    sel_start = TE_SEL_START(te);
    sel_end = TE_SEL_END(te);

    ROMlib_caltext(te, sel_start, 0, &cal_start, &cal_end);
    if(cal_start > sel_start)
        cal_start = sel_start;
    if(cal_end < sel_end)
        cal_end = sel_end;

    TE_DO_TEXT(te, cal_start, cal_end, teDraw);
    TE_DO_TEXT(te, sel_start, sel_end, teHilite);

    TERESTORE();
}

void Executor::C_TESetStyle(int16_t mode, TextStyle *new_attrs, Boolean redraw,
                            TEHandle te)
{
    int16_t start, end;

    if(!TE_STYLIZED_P(te))
        return;

    TE_SLAM(te);

    start = TE_SEL_START(te);
    end = TE_SEL_END(te);

    if(start == end)
    {
        TEStyleHandle te_style;
        StScrpHandle null_scrap;
        ScrpSTElement *scrap_st_elt;

        /* store new attributes into the null scrap style */

        te_style = TE_GET_STYLE(te);
        null_scrap = TE_STYLE_NULL_SCRAP(te_style);
        scrap_st_elt = SCRAP_ST_ELT(null_scrap, 0);
        if(!SCRAP_N_STYLES(null_scrap))
        {
            STHandle style_table;
            StyleRun *runs;
            int start_run_index, start_style_index;

            style_table = TE_STYLE_STYLE_TABLE(te_style);
            runs = TE_STYLE_RUNS(te_style);
            start_run_index = te_char_to_run_index(te_style, start);
            start_style_index = STYLE_RUN_STYLE_INDEX(&runs[start_run_index]);

            /* ### if the scrap has no styles, the old TESetStyle code
	     copied the style from the insertion point.  although this
	     seems reasonable, i haven't been able to find this
	     documented anywhere */
            generic_elt_copy(ST_ELT_TO_GENERIC_ELT(ST_ELT(style_table,
                                                          start_style_index)),
                             SCRAP_ELT_TO_GENERIC_ELT(scrap_st_elt));

            SCRAP_N_STYLES(null_scrap) = 1;
        }
        if(adjust_attrs(SCRAP_ELT_TO_ATTR(scrap_st_elt), new_attrs,
                        SCRAP_ELT_TO_ATTR(scrap_st_elt),
                        SCRAP_ELT_TO_ATTR(scrap_st_elt), mode))
            generic_elt_calc_height_ascent(SCRAP_ELT_TO_GENERIC_ELT(scrap_st_elt));

        return;
    }

    te_add_attrs_to_range(te, start, end, new_attrs, mode);

    if(redraw)
        te_do_redraw(te);

    TE_SLAM(te);
}

void Executor::C_TEReplaceStyle(int16_t mode, TextStyle *attrs_to_replace,
                                TextStyle *replacement_attrs, Boolean redraw,
                                TEHandle te)
{
    TEStyleHandle te_style;
    SignedByte te_style_flags;
    int16_t sel_start, sel_end;
    int16_t start_run_index, end_run_index;
    STHandle style_table;
    StyleRun *runs;
    int16_t run_i;

    TE_SLAM(te);

    sel_start = TE_SEL_START(te);
    sel_end = TE_SEL_END(te);
    if(!TE_STYLIZED_P(te)
       || sel_start == sel_end)
        return;

    te_style = TE_GET_STYLE(te);

    /* create the new runs before locking `te_style' */
    start_run_index = make_style_run_at(te_style, sel_start);
    end_run_index = make_style_run_at(te_style, sel_end);

    te_style_flags = HGetState((Handle)te_style);
    HLock((Handle)te_style);

    runs = TE_STYLE_RUNS(te_style);

    style_table = TE_STYLE_STYLE_TABLE(te_style);

    for(run_i = start_run_index; run_i < end_run_index; run_i++)
    {
        StyleRun *run;
        int16_t orig_style_index;
        STElement *style;

        run = &runs[run_i];
        orig_style_index = STYLE_RUN_STYLE_INDEX(run);
        style = ST_ELT(style_table, orig_style_index);

        /* ### from the IM documentation, it isn't clear if
	 only those attributes specified by `mode' need to match,
	 or if all attributes need to match */

        if((!(mode & doFont)
            || ST_ELT_FONT(style) == TS_FONT(attrs_to_replace))
           && (!(mode & doFace)
               || ST_ELT_FACE(style) == TS_FACE(attrs_to_replace))
           && (!(mode & (doSize | addSize))
               || ST_ELT_SIZE(style) == TS_SIZE(attrs_to_replace))
           && (!(mode & doColor)
               || !memcmp(&ST_ELT_COLOR(style),
                          &TS_COLOR(attrs_to_replace),
                          sizeof(RGBColor))))
        {
            int16_t new_style_index;

            TextStyle *new_attrs = (TextStyle *)alloca(sizeof *new_attrs);

            adjust_attrs(ST_ELT_TO_ATTR(style), replacement_attrs, new_attrs,
                         nullptr, mode);

            /* `get_style_index' may resize `style_table', so `style'
	     is no longer valid */
            new_style_index = get_style_index(te_style, new_attrs, true);
            release_style_index(te_style, orig_style_index);

            STYLE_RUN_STYLE_INDEX(run) = new_style_index;
        }
    }
    HSetState((Handle)te_style, te_style_flags);

    stabilize_style_info(te_style);
    te_style_combine_runs(te_style);

    if(redraw)
        te_do_redraw(te);

    TE_SLAM(te);
}

Boolean Executor::C_TEContinuousStyle(GUEST<INTEGER> *modep, TextStyle *ts_out,
                                      TEHandle teh)
{
    int16_t sel_start, sel_end;
    TEStyleHandle te_style;
    STHandle style_table;
    STElement *style;
    int style_index = -1;
    int run_i;
    GUEST<int16_t> orig_mode;

    if(!TE_STYLIZED_P(teh))
    {
        warning_unimplemented("");
        if(*modep & doFont)
            TS_FONT(ts_out) = PORT_TX_FONT(qdGlobals().thePort);
        if(*modep & doFace)
            TS_FACE(ts_out) = PORT_TX_FACE(qdGlobals().thePort);
        if(*modep & doSize)
            TS_SIZE(ts_out) = PORT_TX_SIZE(qdGlobals().thePort);
        if(*modep & doColor)
            TS_COLOR(ts_out) = ROMlib_black_rgb_color;
        return true;
    }

    /* just slam on entry, this function doesn't modify `teh' */
    TE_SLAM(teh);

    te_style = TE_GET_STYLE(teh);
    style_table = TE_STYLE_STYLE_TABLE(te_style);

    sel_start = TE_SEL_START(teh);
    sel_end = TE_SEL_END(teh);

    orig_mode = *modep;

    /* selection is `insertion point' */
    if(sel_start > sel_end)
    {
        warning_unexpected("start = %d end = %d", sel_start, sel_end);
        sel_start = sel_end;
    }
    if(sel_start == sel_end)
    {
        TextStyle *attr;
        StScrpHandle null_scrap;
        int insertion_point;

        null_scrap = TE_STYLE_NULL_SCRAP(te_style);
        if(SCRAP_N_STYLES(null_scrap))
        {
            ScrpSTElement *scrap_elt;

            scrap_elt = SCRAP_ST_ELT(null_scrap, 0);
            attr = SCRAP_ELT_TO_ATTR(scrap_elt);
        }
        else
        {
            /* get the style of the character preceeding the insertion
	     point; in this case we always return `true', since a single
	     character is contiguous */
            insertion_point = sel_start;
            for(run_i = TE_STYLE_N_RUNS(te_style) - 1; run_i >= 0; run_i--)
            {
                StyleRun *run;

                run = TE_STYLE_RUN(te_style, run_i);
                if(STYLE_RUN_START_CHAR(run) <= std::max(insertion_point - 1, 0))
                {
                    style_index = STYLE_RUN_STYLE_INDEX(run);
                    break;
                }
            }
            /* warning, this pulls out a pointer into an unlocked handle! */
            attr = ST_ELT_TO_ATTR(ST_ELT(style_table, style_index));
        }

        if(*modep & doFont)
            TS_FONT(ts_out) = TS_FONT(attr);
        if(*modep & doFace)
            TS_FACE(ts_out) = TS_FACE(attr);
        if(*modep & doSize)
            TS_SIZE(ts_out) = TS_SIZE(attr);
        if(*modep & doColor)
            TS_COLOR(ts_out) = TS_COLOR(attr);
        return true;
    }
    else
    {
        StyleRun *run = nullptr;
        GUEST<int16_t> font = 0;
        Style face = 0;
        GUEST<int16_t> size = 0;
        RGBColor color;

        /* locate the starting run */
        for(run_i = TE_STYLE_N_RUNS(te_style) - 1; run_i >= 0; run_i--)
        {
            run = TE_STYLE_RUN(te_style, run_i);
            if(STYLE_RUN_START_CHAR(run) < sel_end)
            {
                style_index = STYLE_RUN_STYLE_INDEX(run);
                break;
            }
        }

        /* must have a style */
        gui_assert(style_index > -1
                   && style_index < TE_STYLE_N_STYLES(te_style));
        style = ST_ELT(style_table, style_index);
        if(*modep & doFont)
            font = ST_ELT_FONT(style);
        if(*modep & doFace)
            face = ST_ELT_FACE(style);
        if(*modep & doSize)
            size = ST_ELT_SIZE(style);
        if(*modep & doColor)
            color = ST_ELT_COLOR(style);

        for(; run_i >= 0
            && sel_start < STYLE_RUN_START_CHAR(run);
            run_i--)
        {
            run = TE_STYLE_RUN(te_style, run_i);
            style_index = STYLE_RUN_STYLE_INDEX(run);
            gui_assert(style_index < TE_STYLE_N_STYLES(te_style));
            style = ST_ELT(style_table, style_index);
            if(*modep & doFont
               && font != ST_ELT_FONT(style))
                *modep &= ~doFont;

            face &= ST_ELT_FACE(style);
            if(*modep & doFace
               && !face)
            {
                *modep &= ~doFace;
                TS_FACE(ts_out) = face;
            }

            if(*modep & doSize
               && size != ST_ELT_SIZE(style))
                *modep &= ~doSize;
            if(*modep & doColor
               && !!memcmp(&color, &ST_ELT_COLOR(style), sizeof color))
                *modep &= ~doColor;
        }

        if(*modep & doFont)
            TS_FONT(ts_out) = font;
        if(*modep & doFace)
            TS_FACE(ts_out) = face;
        if(*modep & doSize)
            TS_SIZE(ts_out) = size;
        if(*modep & doColor)
            TS_COLOR(ts_out) = color;

        return orig_mode == *modep;
    }
}

void Executor::C_TEUseStyleScrap(int32_t start, int32_t stop,
                              StScrpHandle newstyles, Boolean redraw,
                              TEHandle teh)
{
    ROMlib_hook(te_notsupported);
    warning_unimplemented("");
}

void Executor::C_TECustomHook(int16_t sel, GUEST<ProcPtr> *addr, TEHandle te)
{
    ROMlib_hook(te_notsupported);
    warning_unimplemented("");
}

LONGINT Executor::C_TENumStyles(int32_t start, int32_t stop, TEHandle te)
{
    ROMlib_hook(te_notsupported);
    warning_unimplemented("");
    return 0;
}
