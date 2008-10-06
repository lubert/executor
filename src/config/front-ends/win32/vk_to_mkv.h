
/* Translate from Win32 VK_ keysyms to the Mac MKV_ keysyms */

#define NOTAKEY	0x89

static unsigned char vk_to_mkv[256] = 
{
				NOTAKEY,
/* VK_LBUTTON	[1] */		NOTAKEY,
/* VK_RBUTTON	[2] */		NOTAKEY,
/* VK_CANCEL	[3] */		NOTAKEY,
/* VK_MBUTTON	[4] */		NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_BACK	[8] */		MKV_BACKSPACE,
/* VK_TAB	[9] */		MKV_TAB,
				NOTAKEY,
				NOTAKEY,
/* VK_CLEAR	[12] */		NOTAKEY,
/* VK_RETURN	[13] */		MKV_RETURN,
				NOTAKEY,
				NOTAKEY,
/* VK_SHIFT	[16] */		MKV_LEFTSHIFT,
/* VK_CONTROL	[17] */		MKV_LEFTCNTL,
/* VK_MENU	[18] */		MKV_CLOVER,
/* VK_PAUSE	[19] */		MKV_PAUSE,
/* VK_CAPITAL	[20] */		MKV_CAPS,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_ESCAPE	[27] */		MKV_ESCAPE,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_SPACE	[32] */		MKV_SPACE,
/* VK_PRIOR	[33] */		MKV_PAGEUP,
/* VK_NEXT	[34] */		MKV_PAGEDOWN,
/* VK_END	[35] */		MKV_END,
/* VK_HOME	[36] */		MKV_HOME,
/* VK_LEFT	[37] */		MKV_LEFTARROW,
/* VK_UP	[38] */		MKV_UPARROW,
/* VK_RIGHT	[39] */		MKV_RIGHTARROW,
/* VK_DOWN	[40] */		MKV_DOWNARROW,
/* VK_SELECT	[41] */		NOTAKEY,
/* VK_PRINT	[42] */		NOTAKEY,
/* VK_EXECUTE	[43] */		NOTAKEY,
/* VK_SNAPSHOT	[44] */		MKV_PRINT_SCREEN,
/* VK_INSERT	[45] */		MKV_HELP,
/* VK_DELETE	[46] */		MKV_DELFORWARD,
/* VK_HELP	[47] */		MKV_HELP,
/* VK_0		[48] */		MKV_0,
/* VK_1		[49] */		MKV_1,
/* VK_2		[50] */		MKV_2,
/* VK_3		[51] */		MKV_3,
/* VK_4		[52] */		MKV_4,
/* VK_5		[53] */		MKV_5,
/* VK_6		[54] */		MKV_6,
/* VK_7		[55] */		MKV_7,
/* VK_8		[56] */		MKV_8,
/* VK_9		[57] */		MKV_9,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_A		[65] */		MKV_a,
/* VK_B		[66] */		MKV_b,
/* VK_C		[67] */		MKV_c,
/* VK_D		[68] */		MKV_d,
/* VK_E		[69] */		MKV_e,
/* VK_F		[70] */		MKV_f,
/* VK_G		[71] */		MKV_g,
/* VK_H		[72] */		MKV_h,
/* VK_I		[73] */		MKV_i,
/* VK_J		[74] */		MKV_j,
/* VK_K		[75] */		MKV_k,
/* VK_L		[76] */		MKV_l,
/* VK_M		[77] */		MKV_m,
/* VK_N		[78] */		MKV_n,
/* VK_O		[79] */		MKV_o,
/* VK_P		[80] */		MKV_p,
/* VK_Q		[81] */		MKV_q,
/* VK_R		[82] */		MKV_r,
/* VK_S		[83] */		MKV_s,
/* VK_T		[84] */		MKV_t,
/* VK_U		[85] */		MKV_u,
/* VK_V		[86] */		MKV_v,
/* VK_W		[87] */		MKV_w,
/* VK_X		[88] */		MKV_x,
/* VK_Y		[89] */		MKV_y,
/* VK_Z		[90] */		MKV_z,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_NUMPAD0	[96] */		MKV_NUM0,
/* VK_NUMPAD1	[97] */		MKV_NUM1,
/* VK_NUMPAD2	[98] */		MKV_NUM2,
/* VK_NUMPAD3	[99] */		MKV_NUM3,
/* VK_NUMPAD4	[100] */	MKV_NUM4,
/* VK_NUMPAD5	[101] */	MKV_NUM5,
/* VK_NUMPAD6	[102] */	MKV_NUM6,
/* VK_NUMPAD7	[103] */	MKV_NUM7,
/* VK_NUMPAD8	[104] */	MKV_NUM8,
/* VK_NUMPAD9	[105] */	MKV_NUM9,
/* VK_MULTIPLY	[106] */	MKV_NUMMULTIPLY,
/* VK_ADD	[107] */	MKV_NUMPLUS,
/* VK_SEPARATOR	[108] */	MKV_NUMCLEAR,	/* ? */
/* VK_SUBTRACT	[109] */	MKV_NUMMINUS,
/* VK_DECIMAL	[110] */	MKV_NUMPOINT,
/* VK_DIVIDE	[111] */	MKV_NUMDIVIDE,
/* VK_F1	[112] */	MKV_F1,
/* VK_F2	[113] */	MKV_F2,
/* VK_F3	[114] */	MKV_F3,
/* VK_F4	[115] */	MKV_F4,
/* VK_F5	[116] */	MKV_F5,
/* VK_F6	[117] */	MKV_F6,
/* VK_F7	[118] */	MKV_F7,
/* VK_F8	[119] */	MKV_F8,
/* VK_F9	[120] */	MKV_F9,
/* VK_F10	[121] */	MKV_F10,
/* VK_F11	[122] */	MKV_F11,
/* VK_F12	[123] */	MKV_F12,
/* VK_F13	[124] */	MKV_F13,
/* VK_F14	[125] */	MKV_F14,
/* VK_F15	[126] */	MKV_F15,
/* VK_F16	[127] */	NOTAKEY,
/* VK_F17	[128] */	NOTAKEY,
/* VK_F18	[129] */	NOTAKEY,
/* VK_F19	[130] */	NOTAKEY,
/* VK_F20	[131] */	NOTAKEY,
/* VK_F21	[132] */	NOTAKEY,
/* VK_F22	[133] */	NOTAKEY,
/* VK_F23	[134] */	NOTAKEY,
/* VK_F24	[135] */	NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* You can only get these keys via GetAsyncKeyState() */
/* VK_NUMLOCK	[144] */	NOTAKEY,
/* VK_SCROLL	[145] */	MKV_SCROLL_LOCK,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* VK_LSHIFT	[160] */	MKV_LEFTSHIFT,
/* VK_RSHIFT	[161] */	MKV_RIGHTSHIFT,
/* VK_LCONTROL	[162] */	MKV_LEFTCNTL,
/* VK_RCONTROL	[163] */	MKV_RIGHTCNTL,
/* VK_LMENU	[164] */	MKV_LEFTOPTION,
/* VK_RMENU	[165] */	MKV_RIGHTOPTION,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* These keys are not given VK_* syms, but were determined experimentally */
/* [186] */			MKV_SEMI,
/* [187] */			MKV_EQUAL,
/* [188] */			MKV_COMMA,
/* [189] */			MKV_MINUS,
/* [190] */			MKV_PERIOD,
/* [191] */			MKV_SLASH,
/* [192] */			MKV_BACKTICK,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
/* [219] */			MKV_LEFTBRACKET,
/* [220] */			MKV_BACKSLASH,
/* [221] */			MKV_RIGHTBRACKET,
/* [222] */			MKV_TICK,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
				NOTAKEY,
};
