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

#include "Xconf.h"

static char *sz_fontNormal = "DejaVu Sans:size=12:antialias=true";
static char *sz_fontTitle = "DejaVu Sans Bold:size=16:antialias=true";

TSXconfig xconf_main;
static XSetWindowAttributes attributes;

static void set_wintitle(const char *win_title) {
    XStoreName(xconf_main.display, xconf_main.win, win_title);
}

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static void create_color(const char *sz_color, XftColor *color) {
    if (!XftColorAllocName(xconf_main.display, xconf_main.visual, xconf_main.cmap, 
                sz_color, color))
        die("cannot allocate xft color\n");
}

static XftFont *get_font(const char* sz_name) {
    XftFont *font = XftFontOpenName(xconf_main.display, xconf_main.screen, sz_name);
    if (!font) {
        die("cannot load font\n");
        return NULL;
    } else {
        return font;
    }
}

bool xconf_open(const int x, const int y, 
                const int width, const int height, 
                const char* win_title) {
    int depth;

    memset(&attributes, 0, sizeof (XSetWindowAttributes));
    xconf_main.display = XOpenDisplay(NULL);
    if (xconf_main.display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    xconf_main.screen = DefaultScreen(xconf_main.display);
    attributes.background_pixel = XWhitePixel(xconf_main.display, xconf_main.screen);
    xconf_main.visual = DefaultVisual(xconf_main.display, xconf_main.screen);
    depth = DefaultDepth(xconf_main.display, xconf_main.screen);
    xconf_main.root_window = RootWindow(xconf_main.display, xconf_main.screen);
    xconf_main.win = XCreateWindow(xconf_main.display, xconf_main.root_window, x, y, width, height, 5, depth, InputOutput,
            xconf_main.visual, CWBackPixel, &attributes);

    XSelectInput(xconf_main.display, xconf_main.win, ExposureMask | KeyPressMask | GCGraphicsExposures | SubstructureNotifyMask);
    xconf_main.gr_values.function = GXcopy;
    xconf_main.gr_values.plane_mask = AllPlanes;
    xconf_main.gr_values.foreground = BlackPixel(xconf_main.display, xconf_main.screen);
    xconf_main.gr_values.background = WhitePixel(xconf_main.display, xconf_main.screen);
    xconf_main.gr_context = XCreateGC(xconf_main.display, xconf_main.win,
            GCClipMask | GCFunction | GCPlaneMask | GCForeground | GCBackground | GCGraphicsExposures,
            &xconf_main.gr_values);

    xconf_main.gc = xconf_main.gr_context;
    XMapWindow(xconf_main.display, xconf_main.win);
    
    set_wintitle(win_title);

    xconf_main.cmap = DefaultColormap(xconf_main.display, xconf_main.screen);
    xconf_main.draw = XftDrawCreate(xconf_main.display, xconf_main.win, xconf_main.visual, xconf_main.cmap);

    xconf_main.fontNormal = get_font(sz_fontNormal);
    xconf_main.fontTitle  = get_font(sz_fontTitle);
    
    create_color("#0000ee", &xconf_main.color);
    create_color("#ffffff", &xconf_main.background);
    create_color("#40ff40", &xconf_main.green);
    return true;
}
