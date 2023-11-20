/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "fixed" };
static const char dmenufont[]       = "fixed";
static const char col_gray1[]       = "#cccccc";
static const char col_gray2[]       = "#cccccc";
static const char col_gray3[]       = "#000000";
static const char col_gray4[]       = "#000000";
static const char col_cyan[]        = "#0066ff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "web" };

static const Rule rules[] = {
	{ 0 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static Key keys[] = {
	/*  modifier        key     function        argument */ \
	{ MODKEY|ShiftMask, XK_c,   killclient,     {0} },
	{ MODKEY,           XK_q,   killclient,     {0} }, \
	{ MODKEY,           XK_F4,  killclient,     {0} }, \
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	{ 0 }
};

