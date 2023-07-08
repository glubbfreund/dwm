/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int fborderpx = 0;        /* border pixel of floating windows */
static const unsigned int gappx	    = 15;
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 4;        /* vertical padding for statusbar */
static const char font[]            = "DejaVuSansM Nerd Font 8";
static const char dmenufont[]       = "DejaVuSansM Nerd Font:size=8";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_blue[]        = "#83cafa";
static const char col_green[]        = "#9ec600";
static const char *colors[][4]      = {
	/*               fg             bg        border   */
	[SchemeNorm] = { col_gray3,   col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4,   col_cyan,  col_cyan  },
	[SchemeMark]  = { col_green,   col_gray1,  col_green  },
	[SchemeTitle]  = { col_blue, col_gray1,  col_cyan  },
};

/* scratchpads */
static const unsigned int use_spc_defaults = 1;
static const int spch             = 768;        /* height of the scratchpad clients */
static const int spcw             = 1200;        /* width of the scratchpad clients */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"thunderbird" , NULL };
const char *spcmd2[] = {"flatpak", "run", "com.spotify.Client", NULL };
const char *spcmd3[] = {"kitty", "--title", "pacmixer", "-e", "pacmixer", NULL };
const char *spcmd4[] = {"kitty", "--title", "PyRadio", "-e", "pyradio", "--log-titles", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"thunderbird",      spcmd1},
	{"Spotify",          spcmd2},
	{"pacmixer",         spcmd3},
	{"PyRadio",          spcmd4},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance  title           tags mask  isfloating  isterminal  noswallow  monitor   notitle   canfocus */
	{ "kitty",          NULL,     NULL,           0,         0,          1,           0,        -1,             0,        1 },
	{ "Gimp",           NULL,     NULL,           0,         1,          0,           0,        -1,             0,        1 },
	{ "Xfce4-notify",   NULL,     NULL,           0,         1,          0,           0,        -1,             1,        0 },
	{ "firefox",        NULL,     NULL,           1,         0,          0,          -1,        -1,             0,        1 },
	{ "libreoffice-",   NULL,     NULL,           0,         1,          0,          -1,        -1,             0,        1 },
	{ "thunderbird",    NULL,     NULL,           SPTAG(0),  1,          0,          -1,        -1,             0,        1 },
	{ "Spotify",        NULL,     NULL,           SPTAG(1),  1,          0,          -1,        -1,             0,        1 },
	{ "kitty",          NULL,  "pacmixer",        SPTAG(2),  1,          0,          -1,        -1,             0,        1 },
	{ "kitty",          NULL,  "PyRadio",         SPTAG(3),  1,          0,          -1,        -1,             0,        1 },
	{ "Blueman-manager",NULL,     NULL,           0,         1,          0,          -1,        -1,             0,        1 },
	{ "Steam",          NULL,     NULL,           0,         1,          0,          -1,        -1,             0,        1 },
	{ "Skype",          NULL,     NULL,           0,         1,          0,          -1,        -1,             0,        1 },
	{ NULL,             NULL, "Event Tester",     0,         0,          0,           1,        -1,             0,        1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󱣲  ",      autotile },    /* no layout function means floating behavior */
	{ "  ",      tile },    /* first entry is default */
	{ "󰎃  ",      monocle },
	{ "󱂬  ",      NULL },    /* no layout function means floating behavior */
};
static const char *monocles[] = { "󰎤  ", "󰎧  ", "󰎪  ", "󰎭  ", "󰎱  ", "󰎳  ", "󰎶  ", "󰎹  ", "󰎼  ", "󰎿  " };

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = { "dmenu_run", "-b", "-i", "-p", "DMenu:", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *dmenushutdowncmd[] = { "/home/oli/Dev/bash/dmenu-shutdowndwm", NULL };
static const char *xkbtoggle[] = { "/home/oli/Src/dwm/scripts/keyset", NULL };
static const char *downvol[] = { "/home/oli/Src/dwm/scripts/vol", "down", NULL };
static const char *brightup[] = { "/home/oli/Src/dwm/scripts/brgt", "up", NULL };
static const char *brightdown[] = { "/home/oli/Src/dwm/scripts/brgt", "down", NULL };
static const char *upvol[] = { "/home/oli/Src/dwm/scripts/vol", "up", NULL };
static const char *mutevol[] = { "/home/oli/Src/dwm/scripts/vol", "mute", NULL };
static const char *printscr[] = { "flameshot", "gui", NULL };
static const char *browser[] = { "firefox", NULL };
static const char *termcmd[]  = { "kitty", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Tab,    spawn,          {.v = xkbtoggle } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browser } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_v,      view,           {0} },
	{ MODKEY|ShiftMask,             XK_e,      spawn,     	   {.v = dmenushutdowncmd} },
    { MODKEY,            			XK_t,  	   togglescratch,  {.ui = 0 } },
    { MODKEY,            			XK_m,  	   togglescratch,  {.ui = 1 } },
    { MODKEY,            			XK_q,  	   togglescratch,  {.ui = 2 } },
    { MODKEY,            			XK_p,  	   togglescratch,  {.ui = 3 } },
    { MODKEY,            			XK_x,  	   togglesticky,  {0} },
	{ MODKEY,                       XK_a,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ 0,                   XK_Print,           spawn,          {.v = printscr } },
	{ 0,                   XF86XK_AudioLowerVolume, spawn,     {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,     {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,     {.v = upvol   } },
	{ 0,                   XF86XK_MonBrightnessUp,  spawn,     {.v = brightup} },
	{ 0,                   XF86XK_MonBrightnessDown,spawn,     {.v = brightdown} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

