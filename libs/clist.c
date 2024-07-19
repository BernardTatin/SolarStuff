/*
 * File:   clist.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 10 mars 2016, 22:12
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

#include <stdlib.h>
#include <stdbool.h>

#include "clist.h"

void cl_list_for_each(TScl_list *list, void (*on_element)(TScl_element *elt)) {
	TScl_element *elt = list->first;

	while (elt != NULL) {
		on_element(elt);
		elt = elt->next;
	}
}

TScl_list *cl_reverse(TScl_list *list) {
	TScl_element *elt = list->first;
    TScl_list *nlist = cl_list_new();

	while (elt != NULL) {
        TScl_element *next = elt->next;

		cl_list_add(nlist, elt);
		elt = next;
	}
    return nlist;
}


void cl_list_free(TScl_list *list, void (*free_value)(void *value)) {
    if (list == NULL) {
        return;
    }
	TScl_element *elt = list->first;

    while (elt != NULL) {
        TScl_element *next = elt->next;
        if (free_value != NULL) {
            free_value(elt->value);
        }
        free(elt);
        elt = next;
    }
    free(list);
}

