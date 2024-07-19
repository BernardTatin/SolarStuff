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

#include <time.h>
#include <sys/time.h>
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

void tei_free(void *vtei) {
    TeInfo *tei = (TeInfo *)vtei;
    if (tei != NULL) {
        free(tei->text);
        free(tei);
    }
}

TScl_list *create_sysinfo_list(void) {
    TScl_list *list = cl_list_new();

    TSsysconf *sc = fill_soli_sysconf();

    tei_new(list,
            "Os %s - %s Machine %s",
            sc->sname.sysname, sc->sname.release, sc->sname.machine);

    tei_new(list,
            "%02d:%02d:%02d",
            sc->tm->tm_hour, sc->tm->tm_min, sc->tm->tm_sec);

    tei_new(list,
            "Procs act. %ld on line %ld",
            sc->num_procs, sc->procs_online);

    tei_new(list,
            "Page size: %ld bytes",
            sc->page_size);

    tei_new(list,
            "Memory: tot: %6ld MB free %6ld MB",
            sc->mem, sc->free_mem);

    tei_new(list,
            "Load %5.2f %5.2f %5.2f",
            sc->load_av[0], sc->load_av[1], sc->load_av[2]);

    return list;
}
