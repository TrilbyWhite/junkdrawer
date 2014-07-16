/* Xkill sample code by Jesse McClure
 * Released to public domain
 *
 * COMPILE:
 * gcc -o xkill xkill.c -lX11
 */

#include <X11/Xlib.h>

int main() {
	Display *dpy = XOpenDisplay(0x0);
	if (!dpy) return 1;
	Window root = DefaultRootWindow(dpy);
	Window ignore, *win;
	unsigned int nwin;
	XQueryTree(dpy, root, &ignore, &ignore, &win, &nwin);
	while (nwin) XKillClient(dpy, win[(--nwin)]);
	if (win) XFree(win);
	XCloseDisplay(dpy);
	return 0;
}
