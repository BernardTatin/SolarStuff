/*
 * File:   Xhelper.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 3 mars 2016, 23:24
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
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <X11/Xft/Xft.h>

#include "Screen.h"
#include "Xhelper.h"


#define MAX_STR_LEN	511

void get_text_extent(TScreen *screen,
        XftFont *font,
        const char *text,
        int *w, int *h) {
    XGlyphInfo extents;
    int len = strlen(text);
    XftTextExtentsUtf8(screen->display, font, (const FcChar8 *)text, len, &extents);
    if (w != NULL) {
        *w = extents.width;
    }
    if (h != NULL) {
        *h = extents.height;
    }
}

int draw_string(TScreen *screen,
        const int x, const int y,
        const char *format, ...) {
    static char buffer[MAX_STR_LEN + 1];
    va_list aptr;
    int ret;

    va_start(aptr, format);
    ret = vsnprintf(buffer, MAX_STR_LEN, format, aptr);
    va_end(aptr);
    buffer[MAX_STR_LEN] = 0;
    if (ret > 0) {
        XftDrawStringUtf8(screen->draw, &screen->color, screen->fontNormal,
            x, y, (const FcChar8 *)buffer, ret);
    }
    return ret;
}
