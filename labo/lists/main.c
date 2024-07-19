/*
 * File:   main.c - testing my list
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 18 juillet 2024, 17:57
 * 
 * Testing the List à la Lisp
 * 
 * Compilation:
 * clang -std=c11 -I../../libs ../../libs/clist.c main.c -o test-list && ./test-list
 * gcc -std=c11 -I../../libs ../../libs/clist.c main.c -o test-list && ./test-list
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

#include <stdio.h>
// to get strdup working 
// after stdio, if not the case, strdup !defined
// because it seems that stdio undef them!!!
#define __USE_XOPEN_EXTENDED
#define __USE_XOPEN2K8
#include <string.h>

#include "clist.h"
#include "sysinfo-list.h"

static void show_elt(TScl_element *elt) {
    char *sz = (char *)elt->value;
    fprintf(stdout, "<%s>\n", sz);
}
static void show_list(TScl_list * list, const char *name) { 
    fprintf(stdout, "%s\n", name);
    cl_list_for_each(list, show_elt);
}

static void show_elt2(TScl_element *elt) {
    TeInfo *tei = (TeInfo *)elt->value;
    char *sz = (char *)tei->text;
    fprintf(stdout, "<%s>\n", sz);
}
static void show_list2(TScl_list * list, const char *name) { 
    fprintf(stdout, "%s\n", name);
    cl_list_for_each(list, show_elt2);
}

int test1(void) {
    char buffer[32];
    TScl_list *list = cl_list_new();

    for (int i=0; i<10; i++) {
        sprintf(buffer, "-- %3d", i);
        TScl_element *elt = cl_elt_new(strdup(buffer));
        cl_list_add(list, elt);
    }
    show_list(list, "First");
    TScl_list *rlist = cl_reverse(list);
    show_list2(rlist, "Reverse");
    return 0;
}

int main(void) {
    TScl_list *lsi = create_sysinfo_list();
    TScl_list *rsi = cl_reverse(lsi);

    show_list2(rsi, "Sysinfo");

    return 0;
}
