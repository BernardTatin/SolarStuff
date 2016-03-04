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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/utsname.h>

#include "Xhelper.h"
#include "Xconf.h"

/*
typedef struct {
	Display* display;
	int screen;
	Window root_window;
	Window win;
	GC gc;
} TSXconfig;

 */

TSXconfig xconf_main;

bool xconf_open(const int x, const int y, const int width, const int height) {
	xconf_main.display = XOpenDisplay(NULL);
	if (xconf_main.display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
	xconf_main.screen = DefaultScreen(xconf_main.display);
	xconf_main.root_window = RootWindow(xconf_main.display, xconf_main.screen);
    xconf_main.win = XCreateSimpleWindow(xconf_main.display, xconf_main.root_window, x, y, width, height, 1,
            BlackPixel(xconf_main.display, xconf_main.screen), WhitePixel(xconf_main.display, xconf_main.screen));
    XSelectInput(xconf_main.display, xconf_main.win, ExposureMask | KeyPressMask);
    XMapWindow(xconf_main.display, xconf_main.win);

	return true;
}
