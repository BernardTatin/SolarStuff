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
#include <X11/Xft/Xft.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <sys/time.h>
#include <poll.h>

#include <sys/utsname.h>

#include "compat.h"

#include "solar-infos.h"
#include "Xconf.h"
#include "Xhelper.h"

#if defined(__APPLE_CC__)
static const char* s1 = "Solar Stuff under Mac OS X Lion";
#elif defined(__FnuxreeBSD__)
static const char* s1 = "Solar Stuff under Linux";
#elif defined(__FreeBSD__)
static const char* s1 = "Solar Stuff under FreeBSD";
#else
static const char* s1 = "Solar Stuff under Solaris";
#endif
static const char* s2 = "(C)2012-2024 Geeks3D - BernardT";

static const int W_WIDTH  = 760;
static const int W_HEIGHT = 220;

static bool onKeyPress(XEvent *e) {
    static char buf[128] = {0};
    KeySym keysym;
    XLookupString(&e->xkey, buf, sizeof buf, &keysym, NULL);
    return (keysym == XK_Escape);
}


static void onExposeMainWindow(const int width,
                               const int height) {
    int x_offset = 10;
    int y_offset = 20;
    int dy = 32;
    TSsysconf *sysconf = soli_sysconf();

    get_text_extent(xconf_main.fontNormal, "Wp|", NULL, &dy);
    dy *= 3;
    dy /= 2;

    XftDrawRect(xconf_main.draw, &xconf_main.background, 0, 0, width, height);
    XhDrawString(x_offset, y_offset, s1);
    y_offset += dy;
    XhDrawString(x_offset, y_offset, s2);
    y_offset += dy;

    if (sysconf->uname_ok) {
        XhDrawString(x_offset, y_offset, "System information -->");
        y_offset += dy;

        XhDrawString(x_offset, y_offset, "- System: %s", sysconf->sname.sysname);
        y_offset += dy;

        XhDrawString(x_offset, y_offset, "- Release: %s", sysconf->sname.release);
        y_offset += dy;

        XhDrawString(x_offset, y_offset, "- Machine: %s", sysconf->sname.machine);
        y_offset += dy;
    }

    x_offset = W_WIDTH/2;
    y_offset = dy;
    XhDrawString(x_offset, y_offset, "%ld CPU%s installed, %ld online", sysconf->num_procs,
            (sysconf->num_procs > 1) ? "s" : "", sysconf->procs_online);
    y_offset += dy;

    XhDrawString(x_offset, y_offset, "%lld MB physical memory, %lld MB free", sysconf->mem, sysconf->free_mem);

    {
        int rwidth = width - x_offset - dy;
        int rpct = (rwidth * sysconf->free_mem) / sysconf->mem;
        int pct  = (100 * sysconf->free_mem) / sysconf->mem;
        int w, h;
        int x, y;
        static char buffer[128];

        sprintf(buffer, "%d %% free", pct);
        get_text_extent(xconf_main.fontNormal, buffer, &w, &h);
        y_offset += dy / 2;
        XftDrawRect (xconf_main.draw, &xconf_main.color,        x_offset,     y_offset,     rwidth + 2, dy + 2);
        XftDrawRect (xconf_main.draw, &xconf_main.background,   x_offset + 1, y_offset + 1, rwidth,     dy);
        XftDrawRect (xconf_main.draw, &xconf_main.green,        x_offset + 1, y_offset + 1, rpct,       dy);
        x = x_offset + (rwidth - w) /2;
        y = y_offset + dy/2 + 4;
        XhDrawString(x, y, "%s", buffer);

        y_offset += 2 * dy;
    }
    XhDrawString(x_offset, y_offset, "average load : %9.2f | %9.2f | %9.2f", sysconf->load_av [LOADAVG_1MIN], sysconf->load_av [LOADAVG_5MIN], sysconf->load_av [LOADAVG_15MIN]);
    y_offset += dy;

    XhDrawString(x_offset, y_offset, "%02d:%02d:%02d", sysconf->tm->tm_hour, sysconf->tm->tm_min, sysconf->tm->tm_sec);
}

static int do_select(const int select_fd) {
    static bool inited = false;
    static struct pollfd poll_list;
    int retval = 0;

    if (!inited) {
        memset(&poll_list, 0, sizeof(struct pollfd));
        poll_list.fd = select_fd;
        poll_list.events = POLLIN | POLLPRI;
        inited = true;
    }
    retval = poll(&poll_list, 1, 800);
    switch(retval) {
        case 0:
            // fprintf(stdout, "poll-> time out\n");
            break;
        case -1:
            fprintf(stdout, "poll-> FATAL ERROR\n");
            exit(2);
            break;
        default:
            // fprintf(stdout, "poll-> event occured\n");
            break;
    }
    return retval;
}


static void send_ExposeEvent(void) {
    static XExposeEvent ee;
    memset(&ee, 0, sizeof (XExposeEvent));
    ee.type = Expose;
    ee.display = xconf_main.display;
    ee.window = xconf_main.win;
    ee.width = W_WIDTH;
    ee.height = W_HEIGHT;
    XSendEvent(xconf_main.display, xconf_main.win, True, ExposureMask, (XEvent *) & ee);
}

static void get_win_title(TSsysconf *sysconf, char *buffer) {
#if defined(__APPLE_CC__)
    strcpy(buffer, "Solar Stuff under Mac OS X (Lion)");
#else
    if (sysconf->uname_ok) {
        sprintf(buffer, "Solar Stuff under Unix (%s)", sysconf->sname.sysname);
    } else {
        strcpy(buffer, "Solar Stuff under Unix");
    }
#endif
}

int main(void) {
    char buffer[512];
    TSsysconf *sysconf;

    soli_start();
    sysconf = soli_sysconf();
    get_win_title(sysconf, buffer);
    xconf_open(100, 100, W_WIDTH, W_HEIGHT, buffer);


    XEvent e;
    bool end = false;

    // https://www.x.org/releases/current/doc/libX11/libX11/libX11.html
    // return a connection number for the specified display. 
    // On a POSIX-conformant system, this is the file descriptor of the connection. 
    int select_fd = XConnectionNumber(xconf_main.display);
    Atom WM_DELETE_WINDOW = XInternAtom(xconf_main.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(xconf_main.display, xconf_main.win, &WM_DELETE_WINDOW, 1);

    XFlush(xconf_main.display);
    while (!end) {
        if (do_select(select_fd)) {
            while (XPending(xconf_main.display)) {
                XNextEvent(xconf_main.display, &e);
                switch (e.type) {
                    case NoExpose:
                        break;
                    case GraphicsExpose:
                        break;
                    case VisibilityNotify:
                        XClearArea(xconf_main.display, xconf_main.win, 0, 0, 0, 1, True);
                        break;
                    case Expose:
                        if (e.xexpose.count == 0) {
                            XExposeEvent *xe = (XExposeEvent *)&e;
                            xconf_init_gc();

                            if (e.xexpose.window == xconf_main.win) {
                                onExposeMainWindow(xe->width,
                                                   xe->height);
                            }
                        }
                        break;
                    case KeyPress:
                        end = onKeyPress(&e);
                        break;
                    case ClientMessage:
                        end = ((unsigned int) (e.xclient.data.l[0]) == WM_DELETE_WINDOW);
                        break;
                    default:
                        // printf("Event 0x%04x - %6d\n", e.type, e.type);
                        break;
                }
            }
        } else {
            send_ExposeEvent();
        }
        XFlush(xconf_main.display);
    }
    soli_stop();
    xconf_close();
    return 0;
}
