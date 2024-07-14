/*
 * File:   private-Xhelper.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 4 mars 2016, 18:46
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

#include <sys/utsname.h>

#include "Screen.h"

static const char *sz_fontNormal = "DejaVu Sans:size=12:antialias=true";
static const char *sz_fontTitle = "DejaVu Sans Bold:size=16:antialias=true";

static void set_wintitle(TScreen *screen, const char *win_title) {
    XStoreName(screen->display, screen->win, win_title);
}

#define MAX_STR_LEN	511
void die(const char *format, ...) {
    static char buffer[MAX_STR_LEN + 1];
    va_list aptr;
    int ret;

    va_start(aptr, format);
    ret = vsnprintf(buffer, MAX_STR_LEN, format, aptr);
    va_end(aptr);
    buffer[MAX_STR_LEN] = 0;
    if (ret > 0) {
        fprintf(stderr, "ERROR: %s\n", buffer);
    } else {
        fprintf(stderr, "UNKNOWN ERROR\n");
    }
    exit(1);
}

static void create_color(TScreen *screen, const char *sz_color, XftColor *color) {
    if (!XftColorAllocName(screen->display,
                screen->visual,
                screen->cmap,
                sz_color, color))
        die("cannot allocate xft color %s\n", sz_color);
}

static XftFont *get_font(TScreen *screen, const char* sz_name) {
    XftFont *font = XftFontOpenName(screen->display, screen->screen, sz_name);
    if (!font) {
        die("cannot load font %s\n", sz_name);
        return NULL;
    } else {
        return font;
    }
}

TScreen *screen_open(const int x, const int y, 
                const int width, const int height, 
                const char* win_title) {
    int depth;
    XSetWindowAttributes attributes;
    TScreen *screen = (TScreen *)malloc(sizeof(TScreen));

    memset(screen, 0, sizeof(TScreen));
    memset(&attributes, 0, sizeof (XSetWindowAttributes));
    screen->display = XOpenDisplay(NULL);
    if (screen->display == NULL) {
        die("Cannot open display\n");
    }
    screen->screen = DefaultScreen(screen->display);
    attributes.background_pixel = XWhitePixel(screen->display, screen->screen);
    screen->visual = DefaultVisual(screen->display, screen->screen);
    depth = DefaultDepth(screen->display, screen->screen);
    screen->root_window = RootWindow(screen->display, screen->screen);
    screen->win = XCreateWindow(screen->display, screen->root_window, x, y, width, height, 5, depth, InputOutput,
            screen->visual, CWBackPixel, &attributes);

    XSelectInput(screen->display, screen->win, ExposureMask | KeyPressMask | GCGraphicsExposures | SubstructureNotifyMask);
    screen->gr_values.function = GXcopy;
    screen->gr_values.plane_mask = AllPlanes;
    screen->gr_values.foreground = BlackPixel(screen->display, screen->screen);
    screen->gr_values.background = WhitePixel(screen->display, screen->screen);
    screen->gr_context = XCreateGC(screen->display, screen->win,
            GCClipMask | GCFunction | GCPlaneMask | GCForeground | GCBackground | GCGraphicsExposures,
            &screen->gr_values);

    screen->gc = screen->gr_context;
    XMapWindow(screen->display, screen->win);

    set_wintitle(screen, win_title);

    screen->cmap = DefaultColormap(screen->display, screen->screen);
    screen->draw = XftDrawCreate(screen->display, screen->win, screen->visual, screen->cmap);

    screen->fontNormal = get_font(screen, sz_fontNormal);
    screen->fontTitle  = get_font(screen, sz_fontTitle);

    create_color(screen, "#0000ee", &screen->color);
    create_color(screen, "#ffffff", &screen->background);
    create_color(screen, "#40ff40", &screen->green);
    return screen;
}
