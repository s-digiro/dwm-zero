/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned long gappx    = 0;        /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static int user_bh            = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */

/*   Display modes of the tab bar: never shown, always shown, shown only in */
/*   monocle mode in presence of several windows.                           */
/*   Modes after showtab_nmodes are disabled                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab            = showtab_auto; /* Default tab bar show mode */
static const Bool toptab            = False;    /* False means bottom tab bar */

static char *fonts[]          = { "terminus:size=12" };
static char dmenufont[]       = "terminus:size=12";
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#202020";
static char normfgcolor[]           = "#ffffff";
static char selfgcolor[]            = "#ffffff";
static char selbordercolor[]        = "#ffffff";
static char selbgcolor[]            = "#000000";
static char titlefgcolor[]          = "#202020";
static char titlebgcolor[]          = "#000000";
static char titlebordercolor[]          = "#202020";
static const char *colors[][3]      = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor },
	[SchemeTitle] = {titlefgcolor, titlebgcolor, titlebordercolor },
};

/* tagging */
#define MAX_TAGLEN 16
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

/* default layout per tags */
/* The first element is for all-tag view, following i-th element corresponds to */
/* tags[i]. Layout is referred using the layouts array index.*/
static int def_layouts[1 + LENGTH(tags)]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered   isfloating   isterminal noswallow monitor */
	{ "St",       NULL,       NULL,       0,            0,           0,           1,         1,        -1 },
	{ "StFloat",  NULL,       NULL,       0,            1,           1,           1,         1,        -1 }
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "tatami.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[+]",      tatami },
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "titlefgcolor",	STRING,  &titlefgcolor },
		{ "titlebgcolor",	STRING,  &titlebgcolor },
		{ "titlebordercolor",	STRING,  &titlebordercolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",      		INTEGER, &snap },
		{ "swallowfloating",    INTEGER, &swallowfloating },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "user_bh",            INTEGER, &user_bh },
		{ "mfact",    	 	FLOAT,   &mfact },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "gappx",              INTEGER, &gappx },
};

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *websitecmd[] = { "website-quick_open", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termfloatcmd[] = { "st", "-c", "StFloat", NULL };
//static const char *browsercmd[] = {"tabbed", "surf", "-e", NULL };
static const char *firefoxcmd[] = {"firefox", NULL};
//static const char *surfcmd[] = {"tabbed", "surf", "-e", NULL};
static const char *vimbcmd[] = {"tabbed", "vimb", "-e", NULL};
static const char *foobar2000cmd[] = {"foobar2000", "start", NULL };
static const char *torbrowsercmd[] = {"torbrowser-launcher", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

#include "movestack.c"
#include "selfrestart.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd} },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = websitecmd} },
	{ MODKEY,                       XK_s,      spawn,          {.v = termcmd} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = termfloatcmd} },
	{ MODKEY,                       XK_f,      spawn,          {.v = firefoxcmd} },
	{ MODKEY,                       XK_a,      spawn,          {.v = foobar2000cmd} },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = torbrowsercmd} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
	{ MODKEY,			XK_h,      focusmaster,    {0} },
	{ MODKEY,			XK_Left,   focusmaster,    {0} },
	{ MODKEY,			XK_l,      focusstack,     {.i = +1} },
	{ MODKEY,			XK_Right,  focusstack,     {.i = +1} },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Left,   incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Right,  incnmaster,     {.i = -1 } },
	//{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	//{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Down,   movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Up,     movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_q,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_space,  togglefullscr,  {0} },
	{ MODKEY|Mod1Mask,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod1Mask,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod1Mask,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|Mod1Mask|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|Mod1Mask|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|Mod1Mask|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	//{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	//{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	//{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	//{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	//{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	//{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,			XK_n,      nametag,        {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(			XK_0,                      9)
	{ MODKEY|ShiftMask,             XK_r,           self_restart,   {0} },
	{ MODKEY|ShiftMask,             XK_Escape,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask          button          function             argument */
	{ ClkLtSymbol,          0,                  Button1,        setlayout,           {0} },
	{ ClkLtSymbol,          0,                  Button3,        layoutmenu,          {0} },
	{ ClkWinTitle,          0,                  Button2,        zoom,                {0} },
	{ ClkStatusText,        0,                  Button2,        spawn,               {.v = termcmd } },
	{ ClkClientWin,         MODKEY,             Button1,        movemouse,           {0} },
	{ ClkClientWin,         MODKEY,             Button2,        togglefloating,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,   Button1,        resizemouse,         {0} },
	{ ClkClientWin,         MODKEY|Mod1Mask,    Button1,        resizemfactmouse,    {0} },
	{ ClkTagBar,            0,                  Button1,        view,                {0} },
	{ ClkTagBar,            0,                  Button3,        toggleview,          {0} },
	{ ClkTagBar,            MODKEY,             Button1,        tag,                 {0} },
	{ ClkTagBar,            MODKEY,             Button3,        toggletag,           {0} },
	{ ClkTabBar,            0,                  Button1,        focuswin,            {0} },
};

