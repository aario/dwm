/* See LICENSE file for copyright and license details. */

#define NUMCOLORS         5
static const char colors[NUMCOLORS][MAXCOLORS][8] = {
  // border   foreground background
  { "#252525", "#CFFFEA", "#252525" },  // normal
  { "#252525", "#B87201", "#252525" },  // selected
  { "#FFCA00", "#000000", "#FFCA00" },  // urgent/warning  (black on yellow)
  { "#ff0000", "#ffffff", "#ff0000" },  // error (white on red)
  { "#252525", "#E08A00", "#252525" },  // Active titlebar
  // add more here
};

/* Delimiter character for status text
 * Whatever is before delimiter is normal status text whatever comes
 * after it will be displayed on hover. When delimiter character doesn't
 * exist, normal text will be always displayed.
 * For example if delimiter is $ and:
 * 	xsetroot -name 'abcd$defg'
 * Then the status text will be:
 * 	abcd
 * And on mouse hover it becomes:
 * 	defg
 * */
static const char stext_delimiter = '$';

/* appearance */
static const char *fonts[] = {
	"monospace:size=15"
};
static const char dmenufont[]       = "monospace:size=15";
static const char normbordercolor[] = "#252525";
static const char firstbgcolor[]     = "#252525";
static const char firstfgcolor[]     = "#CFFFEA";
static const char normbgcolor[]     = "#252525";
static const char normfgcolor[]     = "#CFFFEA";
static const char selbordercolor[]  = "#E08A00";
static const char selbgcolor[]      = "#E08A00";
static const char selfgcolor[]      = "#00FFFC";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 18;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
//	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "xFirefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|||",      col },
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
static const char *dmenucmd_prefix          ="dmenu_";
static const char *dmenuallcmd[]            = { "dmenu_all", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenucmd[]               = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuwincmd[]            = { "dmenu_win", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuvolcmd[]            = { "dmenu_vol", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuvolcmd_down[]       = { "dmenu_vol", "--volumedown", NULL };
static const char *dmenuvolcmd_mute[]       = { "dmenu_vol", "--mute", NULL };
static const char *dmenuvolcmd_up[]         = { "dmenu_vol", "--volumeup", NULL };
static const char *dmenublcmd[]             = { "dmenu_bl", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenublcmd_down[]        = { "dmenu_bl", "--backlightdown", NULL };
static const char *dmenublcmd_up[]          = { "dmenu_bl", "--backlightup", NULL };
static const char *dmenumediacmd[]          = { "dmenu_media", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenumediacmd_playpause[]= { "dmenu_media", "--playpause", NULL };
static const char *dmenumediacmd_stop[]     = { "dmenu_media", "--stop", NULL };
static const char *dmenumediacmd_previous[] = { "dmenu_media", "--previous", NULL };
static const char *dmenumediacmd_next[]     = { "dmenu_media", "--next", NULL };
static const char *dmenucustomcmd[]         = { "dmenu_custom", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuhomecmd[]           = { "dmenu_home", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuappscmd[]           = { "dmenu_apps", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *slockcmd[]               = { "slock", NULL };
static const char *termcmd[]                = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = dmenuwincmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = dmenuvolcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = dmenublcmd } },
	{ MODKEY,                       XK_m,      spawn,          {.v = dmenumediacmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = dmenucustomcmd } },
	{ MODKEY,                       XK_h,      spawn,          {.v = dmenuhomecmd } },
	{ MODKEY,                       XK_Escape, spawn,          {.v = dmenuappscmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_l,      spawn,          {.v = slockcmd } },
	{ MODKEY,                       XK_x,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,      pushdown,       {0} },
	{ MODKEY|ControlMask,           XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ Mod1Mask,             				XK_F4,     killclient,     {0} },
	{ MODKEY,                       XK_F1,     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_F2,     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_F3,     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_F4,     setlayout,      {.v = &layouts[3]} },
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
	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0,                            KEYSYM_VOLUMEDOWN,spawn,   {.v = dmenuvolcmd_down } },
	{ 0,                            KEYSYM_MUTE,spawn,         {.v = dmenuvolcmd_mute } },
	{ 0,                            KEYSYM_VOLUMEUP,spawn,     {.v = dmenuvolcmd_up } },
	{ 0,                            KEYSYM_HOME,spawn,         {.v = dmenuhomecmd } },
	{ 0,                            KEYSYM_MEDIAPLAYER,spawn,  {.v = dmenumediacmd } },
	{ 0,                            KEYSYM_PLAYPAUSE,spawn,    {.v = dmenumediacmd_playpause } },
	{ 0,                            KEYSYM_STOP,spawn,         {.v = dmenumediacmd_stop } },
	{ 0,                            KEYSYM_PREVIOUS,spawn,     {.v = dmenumediacmd_previous } },
	{ 0,                            KEYSYM_NEXT,spawn,         {.v = dmenumediacmd_next } },
	{ 0,                            KEYSYM_BACKLIGHTDOWN,spawn,{.v = dmenublcmd_down } },
	{ 0,                            KEYSYM_BACKLIGHTUP,spawn,  {.v = dmenublcmd_up } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

static const char AUTOSTART_CMD[] = "~/.dwm/autostart.sh &";
static const char AUTOSTART_BLOCKING_CMD[] = "~/.dwm/autostart_blocking.sh";
/* intensity of blur level*/
static unsigned int blurlevel  = 5;

//Used for multi-threaded blur effect
#define CPU_THREADS 4 
