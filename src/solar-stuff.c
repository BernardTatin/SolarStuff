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

#include "Xconf.h"
#include "Xhelper.h"

#if defined(__APPLE_CC__)
static const char* s1 = "X11 test app under Mac OS X Lion";
#else
static const char* s1 = "X11 test app under Solaris";
#endif
static const char* s2 = "(C)2012 Geeks3D.com";

static struct utsname sname;
static bool uname_ok;

static bool onKeyPress(XEvent *e) {
    static char buf[128] = {0};
    KeySym keysym;
    XLookupString(&e->xkey, buf, sizeof buf, &keysym, NULL);
    return (keysym == XK_Escape);
}

static void onExpose(Display *display, const int screen, const Window win) {
    GC gc = xconf_init_gc();
    int y_offset = 20;

    XhDrawString(10, y_offset, (char *) s1);
    y_offset += 20;
    XhDrawString(10, y_offset, (char *) s2);
    y_offset += 20;
    if (uname_ok) {
        XhDrawString(10, y_offset, "System information -->");
        y_offset += 15;

        XhDrawString(10, y_offset, "- System: %s", sname.sysname);
        y_offset += 15;

        XhDrawString(10, y_offset, "- Release: %s", sname.release);
        y_offset += 15;

        XhDrawString(10, y_offset, "- Version: %s", sname.version);
        y_offset += 15;

        XhDrawString(10, y_offset, "- Machine: %s", sname.machine);
        y_offset += 20;
    }


    XWindowAttributes wa;
    XGetWindowAttributes(xconf_main.display, xconf_main.win, &wa);
    int width = wa.width;
    int height = wa.height;

    XhDrawString(10, y_offset, "Current window size: %dx%d", width, height);
}

int main(int argc, char** argv) {
	char buffer[128];
	xconf_open(100, 100, 660, 200);
    uname_ok = uname(&sname) != -1;

#if defined(__APPLE_CC__)
    y_offset += 15;
    XStoreName(display, win, "Geeks3D.com - X11 window under Mac OS X (Lion)");
#else
	if (uname_ok) {
		sprintf(buffer, "Geeks3D.com - X11 window under Unix (%s)", sname.sysname);
	} else {
		strcpy(buffer, "Geeks3D.com - X11 window under Unix");
	}
    XStoreName(xconf_main.display, xconf_main.win, buffer);
#endif
    Atom WM_DELETE_WINDOW = XInternAtom(xconf_main.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(xconf_main.display, xconf_main.win, &WM_DELETE_WINDOW, 1);

    XEvent e;
    bool end = false;

    while (!end) {
        XNextEvent(xconf_main.display, &e);
        switch (e.type) {
            case Expose:
                onExpose(xconf_main.display, xconf_main.screen, xconf_main.win);
                break;
            case KeyPress:
                end = onKeyPress(&e);
                break;
            case ClientMessage:
                end = ((unsigned int) (e.xclient.data.l[0]) == WM_DELETE_WINDOW);
                break;
        }
    }
	xconf_close();
    return 0;
}