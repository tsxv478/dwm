/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 30;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int enablegaps               = 1;        /* 1 means enable gaps on startup */
static int smartgaps                = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int barheight          = 2;        /* 2 is the default spacing around the bar's font */
static const char *fonts[]          = { "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#008888";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray2, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title      tags mask  isfloating  isterminal  noswallow  monitor */
	{ "st-256color",   NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,            NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define PERTAG_GAPS 1
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[]^",      bstack },
	{ "[0]",      monocle },
	{ "=[]=",     centeredmaster },
	{ "[]@",      spiral },
	{ "[]\\",     dwindle },
	{ "[][",      deck },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray2, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key           function        argument */
	STACKKEYS(MODKEY,                             focus)
	STACKKEYS(MODKEY|ShiftMask,                   push)
	{ MODKEY,                       XK_d,         spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_space,     spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_equal,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_minus,     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_l,         shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_h,         shiftview,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,    zoom,           {0} },
	{ MODKEY,                       XK_Tab,       view,           {0} },
	{ MODKEY,                       XK_q,         killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_u,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_i,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_o,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_p,         setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,           XK_u,         setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_i,         setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ControlMask,           XK_o,         setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ControlMask,           XK_p,         setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_f,         togglefloating, {0} },
	{ MODKEY|ControlMask,           XK_f,         togglefullscr,  {0} },
	{ MODKEY,                       XK_grave,     togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_grave,     togglebar,      {0} },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
	{ MODKEY|ShiftMask,             XK_Escape,    quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

