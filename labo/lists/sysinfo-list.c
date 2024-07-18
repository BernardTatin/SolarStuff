/* 
 * File:   sysinfo-list.c
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

#include <sys/utsname.h>
#include <stdio.h>
// to get strdup working 
// after stdio, if not the case, strdup !defined
// because it seems that stdio undef them!!!
#define __USE_XOPEN_EXTENDED
#define __USE_XOPEN2K8
#include <string.h>

#include "sysinfo-list.h"
#include "compat.h"

#include "solar-infos.h"

TScl_list *create_sysinfo_list(void) {
    TScl_list *list = cl_list_new();
    TSsysconf *sc = soli_sysconf();

    TScl_element *e = tei_new("Procs act. %ld on line %ld", 
        sc->num_procs, sc->procs_online);
    cl_list_add(list, e);

    e = tei_new("Page size: %ld bytes", sc->page_size);
    cl_list_add(list, e);

    return list;
}