/* 
 * File:   sysinfo-list.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 10 mars 2016, 21:57
 * 
 * List à la Lisp
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

#if !defined(__sysinfo_list_h_)
#define __sysinfo_list_h_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "clist.h"

typedef struct {
    char *text;
    int text_len;
    bool warning;
} TeInfo;

#define MAX_STR_LEN	511
static inline TScl_element *tei_new(const char *format, ...) {
    static char buffer[MAX_STR_LEN + 1];
    va_list aptr;
    int ret;
    TeInfo *e = (TeInfo *)calloc(1, sizeof(TeInfo));

    va_start(aptr, format);
    ret = vsnprintf(buffer, MAX_STR_LEN, format, aptr);
    va_end(aptr);
    buffer[MAX_STR_LEN] = 0;
    e->text = strdup(buffer);
    e->text_len = strlen(buffer);
    e->warning = ret != e->text_len;
    fprintf(stdout, "%s -> %s\n", buffer, e->text);
    return cl_elt_new(e);
}

TScl_list *create_sysinfo_list(void);

#endif // __sysinfo_list_h_
