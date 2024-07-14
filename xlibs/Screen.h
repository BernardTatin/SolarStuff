/* 
 * File:   private-Xhelper.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 4 mars 2016, 18:36
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


#ifndef PRIVATE_XHELPER_H
#define	PRIVATE_XHELPER_H

typedef struct {
	Display*    display;
	int         screen;
	Window      root_window;
	Window      win;
	GC          gc;
	XGCValues   gr_values; 
	GC          gr_context;

    XftFont  *fontNormal;
    XftFont  *fontTitle;
    Visual   *visual;
    XftDraw  *draw;
    XftColor  color;
    XftColor  background;
    XftColor  green;
    Colormap  cmap;
} TScreen;

TScreen *screen_open(const int x, const int y,
                const int width, const int height,
                const char* win_title);

static inline GC screen_init_gc(TScreen *screen) {
	screen->gc = DefaultGC(screen->display, screen->screen);
	screen->gc = screen->gr_context;
	return screen->gc;
}


static inline void screen_close(TScreen *screen) {
    XDestroyWindow(screen->display, screen->win);
    XCloseDisplay(screen->display);
}

void die(const char *format, ...);

#endif	/* PRIVATE_XHELPER_H */

