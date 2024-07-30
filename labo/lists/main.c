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
#include <string.h>
#include <unistd.h>

#include "clist.h"
#include "sysinfo-list.h"

static void show_elt(TScl_element *elt) {
    char *sz = (char *)elt->value;
    fprintf(stdout, "<%s>\n", sz);
}

static void show_elt2(TScl_element *elt) {
    TeInfo *tei = (TeInfo *)elt->value;
    char *sz = (char *)tei->text;
    fprintf(stdout, "<%s>\n", sz);
}

static void show_list(TScl_list * list, 
            const char *name,
            void (*on_element)(TScl_element *elt)) { 
    fprintf(stdout, "%s\n", name);
    cl_list_for_each(list, on_element);
}


static void kill_lists(TScl_list *lsi, 
                    TScl_list *rsi, 
                    void (*free_value)(void *value)) {
    // cl_list_free(lsi, tei_free);
    free(lsi);
    cl_list_free(rsi, free_value);
}

int test1(void) {
    char buffer[32];
    TScl_list *list = cl_list_new();

    for (int i=0; i<10; i++) {
        sprintf(buffer, "-- %3d", i);
        TScl_element *elt = cl_elt_new(strdup(buffer));
        cl_list_add(list, elt);
    }
    show_list(list, "First", show_elt);
    TScl_list *rlist = cl_reverse(list);
    show_list(rlist, "Reverse", show_elt);
    kill_lists(list, rlist, free);
    return 0;
}

int main(void) {
    test1();

    for (int i=0; i<5; i++) {
        TScl_list *lsi = create_sysinfo_list();
        TScl_list *rsi = cl_reverse(lsi);

        fprintf(stdout, "----------------------------------------------------------------------\n");
        show_list(rsi, "Sysinfo", show_elt2);
        kill_lists(lsi, rsi, tei_free);
        sleep(1);
    }

    return 0;
}
