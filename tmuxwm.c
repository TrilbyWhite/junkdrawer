/* TMUXWM.C
 * Copyright Jesse McClure 2015
 * License: GPLv3
 * Compile: gcc -o tmuxwm tmuxwm.c -lX11
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

typedef struct Bind { KeySym sym; int mod; const char * const *args; } Bind;
typedef const char * const Arg[];

/* The next ~30 lines could be separated into a "config.h", but this is BMFM
 * code: By Me For Me.  So I don't care to separate it for your convenience.
 */

Arg tmux_client = (Arg) { "urxvt", "-e", "tmux", "new-session", "-AD", "-s", "TmuxWM", NULL };

#include <X11/XF86keysym.h>
static Bind bind[] = {
	/* Windows */
	{ XK_Return,          Mod4Mask, (Arg) { "tmux", "new-window", NULL } },
	{ XK_Tab,             Mod4Mask, (Arg) { "tmux", "next-window", NULL } },
	{ XK_grave,           Mod4Mask, (Arg) { "tmux", "last-window", NULL } },
	{ XK_1,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "1",  NULL } },
	{ XK_2,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "2",  NULL } },
	{ XK_3,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "3",  NULL } },
	{ XK_4,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "4",  NULL } },
	{ XK_5,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "5",  NULL } },
	{ XK_6,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "6",  NULL } },
	{ XK_7,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "7",  NULL } },
	{ XK_8,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "8",  NULL } },
	{ XK_9,               Mod4Mask, (Arg) { "tmux", "select-window", "-t", "9",  NULL } },
	/* Panes */
	{ XK_h,               Mod4Mask, (Arg) { "tmux", "split-window", "-h", NULL} },
	{ XK_v,               Mod4Mask, (Arg) { "tmux", "split-window", "-v", NULL} },
	{ XK_space,           Mod4Mask, (Arg) { "tmux", "last-pane", NULL } },
	/* Launchers / Misc */
	{ XK_w,               Mod4Mask,    (Arg) { "firefox", NULL} },
	{ XK_Menu,                      0, (Arg) { "interrobang", NULL } },
	{ XF86XK_AudioRaiseVolume,      0, (Arg) { "amixer", "set", "Master", "2+", NULL} },
	{ XF86XK_AudioLowerVolume,      0, (Arg) { "amixer", "set", "Master", "2-", NULL} },
	{ XF86XK_AudioMute,             0, (Arg) { "amixer", "set", "Master", "toggle", NULL} },
	{ XF86XK_ScreenSaver,           0, (Arg) { "slock", NULL} },
};

/* End of potential "config.h" content
 */

static void buttonpress(XEvent *);
static void configurerequest(XEvent *);
static void enternotify(XEvent *);
static void keypress(XEvent *);
static void maprequest(XEvent *);
static void unmapnotify(XEvent *);

static Display *dpy;
static Window root;
static int sw, sh;
static void (*handler[LASTEvent])(XEvent *) = {
	[ButtonPress]        = buttonpress,
	[ConfigureRequest]   = configurerequest,
	[EnterNotify]        = enternotify,
	[KeyPress]           = keypress,
	[MapRequest]         = maprequest,
};

static int xerror(Display *d, XErrorEvent *e) {
	char msg[256];
	XGetErrorText(d, e->error_code, msg, sizeof(msg));
	fprintf(stderr,"[X11 %d:%d] %s\n", e->request_code, e->error_code, msg);
}

static inline int spawn(Arg args) {
	if (fork() != 0) return 1;
	close(ConnectionNumber(dpy));
	setsid();
	execvp(args[0], args);
}

int main(int argc, const char **argv) {
	signal(SIGCHLD, SIG_IGN);
	if(!(dpy = XOpenDisplay(0x0))) return 1;
	XSetErrorHandler(xerror);
	root = DefaultRootWindow(dpy);
	sw = DisplayWidth(dpy, DefaultScreen(dpy));
	sh = DisplayHeight(dpy, DefaultScreen(dpy));
	XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Tab), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Return), Mod4Mask|ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
	int i;
	for (i = 0; i < sizeof(bind)/sizeof(bind[0]); ++i)
		XGrabKey(dpy, XKeysymToKeycode(dpy, bind[i].sym), bind[i].mod, root, True, GrabModeAsync, GrabModeAsync);
	XGrabButton(dpy, AnyButton, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
	XSelectInput(dpy, root, SubstructureNotifyMask | SubstructureRedirectMask);
	XEvent ev;
	spawn(tmux_client);
	while (!XNextEvent(dpy,&ev))
		if (handler[ev.type]) handler[ev.type](&ev);
	return 0;
}

void buttonpress(XEvent *ev) {
	XButtonEvent *e = &ev->xbutton;
	XRaiseWindow(dpy, e->subwindow);
	if (e->button == 2)
		XMoveResizeWindow(dpy, e->subwindow, 0, 0, sw, sh);
	if (e->button != 1 && e->button !=3)
		return;
	XGrabPointer(dpy, root, True, PointerMotionMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	XWindowAttributes attr;
	XGetWindowAttributes(dpy, e->subwindow, &attr);
	int xx = e->x_root, yy = e->y_root, px, py;
	XEvent ee;
	while (!XMaskEvent(dpy, PointerMotionMask | ButtonReleaseMask, &ee)) {
		if (ee.type == ButtonRelease) break;
		px = xx; py = yy; xx = ee.xbutton.x_root; yy = ee.xbutton.y_root;
		if (e->button == 1) { attr.x += xx - px; attr.y += yy - py; }
		else if (e->button == 3) {
			attr.width += xx - px; attr.height += yy - py;
			if (attr.width < 12) attr.width = 12;
			else if (attr.height < 12) attr.height = 12;
		}
		XMoveResizeWindow(dpy, e->subwindow, attr.x, attr.y, attr.width, attr.height);
	}
	XUngrabPointer(dpy, CurrentTime);
}

void configurerequest(XEvent *ev) {
	XConfigureRequestEvent *e = &ev->xconfigurerequest;
	XWindowChanges wc;
	wc.x = e->x; wc.y = e->y; wc.width = e->width; wc.height = e->height;
	wc.sibling = Above; wc.stack_mode = e->detail; wc.border_width = e->border_width;
	XConfigureWindow(dpy, e->window, e->value_mask, &wc);
}

void enternotify(XEvent *ev) {
	XSetInputFocus(dpy, ev->xcrossing.window, RevertToPointerRoot, CurrentTime);
}

void keypress(XEvent *ev) {
	XKeyEvent *e = &ev->xkey;
	KeySym sym = XkbKeycodeToKeysym(dpy, e->keycode, 0, 0);
	int i;
	if (e->state == Mod1Mask && sym == XK_Tab)
		XCirculateSubwindowsUp(dpy, root);
	else if (e->state == (Mod4Mask|ShiftMask) && sym == XK_Return)
		spawn(tmux_client);
	else for (i = 0; i < sizeof(bind)/sizeof(bind[0]); ++i)
		if (bind[i].sym == sym && bind[i].mod == e->state && bind[i].args)
				spawn(bind[i].args);
}

#define MapFocus		0x01
#define MapFull		0x02
void maprequest(XEvent *ev) {
	XMapRequestEvent *e = &ev->xmaprequest;
	XWindowAttributes wa;
	if (!XGetWindowAttributes(dpy, e->window, &wa) || wa.override_redirect)
		return;
	int flags = MapFocus | MapFull;
	XWMHints *wmHint;
	if ( (wmHint=XGetWMHints(dpy,e->window)) ) {
		if (wmHint->flags & InputHint && !wmHint->input) flags &= ~MapFocus;
		XFree(wmHint);
	}
	XClassHint csHint;
	XGetClassHint(dpy, e->window, &csHint);
	if (strncmp(csHint.res_name,"Float",5) == 0) flags &= ~MapFull;
	if (strncmp(csHint.res_class,"Float",5) == 0) flags &= ~MapFull;
	Window parent;
	if (XGetTransientForHint(dpy, e->window, &parent)) flags &= ~MapFull;
	if (flags & MapFocus) XSelectInput(dpy, e->window, EnterWindowMask);
	if (flags & MapFull) XMoveResizeWindow(dpy, e->window, 0, 0, sw, sh);
	else XMoveResizeWindow(dpy, e->window, wa.x, wa.y, wa.width, wa.height);
	XMapWindow(dpy, e->window);
}

