/*
 * File:   solar-stuff.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 3 mars 2016, 22:20
 */

/*
    The MIT License (MIT)

    Copyright (c) 2016 Bernard TATIN <bernard dot tatin at outlook dot org>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

/*
 * http://www.geeks3d.com/20120102/programming-tutorial-simple-x11-x-window-code-sample-for-linux-and-mac-os-x/
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/utsname.h>

int main(int argc, char** argv)
{
	Display* dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}
	int s = DefaultScreen(dpy);
	Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, s), 10, 10, 660, 200, 1,
			BlackPixel(dpy, s), WhitePixel(dpy, s));
	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);

#if defined(__APPLE_CC__)
	XStoreName(dpy, win, "Geeks3D.com - X11 window under Mac OS X (Lion)");
#else
	XStoreName(dpy, win, "Geeks3D.com - X11 window under Linux (Mint 10)");
#endif
	Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);

	bool uname_ok = false;
	struct utsname sname;
	int ret = uname(&sname);
	if (ret != -1)
	{
		uname_ok = true;
	}
	XEvent e;
	while (1)
	{
		XNextEvent(dpy, &e);
		if (e.type == Expose)
		{
			int y_offset = 20;
#if defined(__APPLE_CC__)
			const char* s1 = "X11 test app under Mac OS X Lion";
#else
			const char* s1 = "X11 test app under Solaris";
#endif
			const char* s2 = "(C)2012 Geeks3D.com";
			XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, s1, strlen(s1));
			y_offset += 20;
			XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, s2, strlen(s2));
			y_offset += 20;

			if (uname_ok)
			{
				char buf[256] = {0};
				sprintf(buf, "System information:");
				XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
				y_offset += 15;

				sprintf(buf, "- System: %s", sname.sysname);
				XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
				y_offset += 15;

				sprintf(buf, "- Release: %s", sname.release);
				XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
				y_offset += 15;

				sprintf(buf, "- Version: %s", sname.version);
				XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
				y_offset += 15;
				sprintf(buf, "- Machine: %s", sname.machine);
				XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
				y_offset += 20;
			}


			XWindowAttributes wa;
			XGetWindowAttributes(dpy, win, &wa);
			int width = wa.width;
			int height = wa.height;
			char buf[128]={0};
			sprintf(buf, "Current window size: %dx%d", width, height);
			XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
			y_offset += 20;
		}
		if (e.type == KeyPress)
		{
			char buf[128] = {0};
			KeySym keysym;
			int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
			if (keysym == XK_Escape)
				break;
		}

		if ((e.type == ClientMessage) &&
				((unsigned int)(e.xclient.data.l[0]) == WM_DELETE_WINDOW))
		{
			break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 0;
}
