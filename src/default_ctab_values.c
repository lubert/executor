/* Copyright 1994 by Abacus Research and
 * Development, Inc.  All rights reserved.
 */

#if !defined (OMIT_RCSID_STRINGS)
char ROMlib_rcsid_default_ctab_values[] =
		"$Id: default_ctab_values.c 63 2004-12-24 18:19:43Z ctm $";
#endif

#include "rsys/common.h"
#include "QuickDraw.h"

ColorSpec ctab_1bpp_values[] =
{
  { CWC (0), { CWC (65535), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (0) } }, 
};

ColorSpec ctab_2bpp_values[] =
{
  { CWC (0), { CWC (65535), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (44204), CWC (44204), CWC (44204) } }, 
  { CWC (0), { CWC (21845), CWC (21845), CWC (21845) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (0) } }, 
};

ColorSpec ctab_4bpp_values[] =
{
  { CWC (0), { CWC (65535), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (64512), CWC (62333), CWC (1327) } }, 
  { CWC (0), { CWC (65535), CWC (25738), CWC (652) } }, 
  { CWC (0), { CWC (56683), CWC (2242), CWC (1698) } }, 
  { CWC (0), { CWC (62167), CWC (2134), CWC (34028) } }, 
  { CWC (0), { CWC (18147), CWC (0), CWC (42302) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (54272) } }, 
  { CWC (0), { CWC (577), CWC (43860), CWC (60159) } }, 
  { CWC (0), { CWC (7969), CWC (46995), CWC (5169) } }, 
  { CWC (0), { CWC (0), CWC (25775), CWC (4528) } }, 
  { CWC (0), { CWC (22016), CWC (11421), CWC (1316) } }, 
  { CWC (0), { CWC (37079), CWC (29024), CWC (14900) } }, 
  { CWC (0), { CWC (49152), CWC (49152), CWC (49152) } }, 
  { CWC (0), { CWC (32768), CWC (32768), CWC (32768) } }, 
  { CWC (0), { CWC (16384), CWC (16384), CWC (16384) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (0) } }, 
};

ColorSpec ctab_8bpp_values[] =
{
  { CWC (0), { CWC (65535), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (65535), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (52428), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (39321), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (26214), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (13107), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (13107) } }, 
  { CWC (0), { CWC (0), CWC (65535), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (13107) } }, 
  { CWC (0), { CWC (0), CWC (52428), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (13107) } }, 
  { CWC (0), { CWC (0), CWC (39321), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (13107) } }, 
  { CWC (0), { CWC (0), CWC (26214), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (13107) } }, 
  { CWC (0), { CWC (0), CWC (13107), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (65535) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (52428) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (39321) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (26214) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (13107) } }, 
  { CWC (0), { CWC (61166), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (56797), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (48059), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (43690), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (34952), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (30583), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (21845), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (17476), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (8738), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (4369), CWC (0), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (61166), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (56797), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (48059), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (43690), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (34952), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (30583), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (21845), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (17476), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (8738), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (4369), CWC (0) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (61166) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (56797) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (48059) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (43690) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (34952) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (30583) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (21845) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (17476) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (8738) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (4369) } }, 
  { CWC (0), { CWC (61166), CWC (61166), CWC (61166) } }, 
  { CWC (0), { CWC (56797), CWC (56797), CWC (56797) } }, 
  { CWC (0), { CWC (48059), CWC (48059), CWC (48059) } }, 
  { CWC (0), { CWC (43690), CWC (43690), CWC (43690) } }, 
  { CWC (0), { CWC (34952), CWC (34952), CWC (34952) } }, 
  { CWC (0), { CWC (30583), CWC (30583), CWC (30583) } }, 
  { CWC (0), { CWC (21845), CWC (21845), CWC (21845) } }, 
  { CWC (0), { CWC (17476), CWC (17476), CWC (17476) } }, 
  { CWC (0), { CWC (8738), CWC (8738), CWC (8738) } }, 
  { CWC (0), { CWC (4369), CWC (4369), CWC (4369) } }, 
  { CWC (0), { CWC (0), CWC (0), CWC (0) } }, 
};
