/* 
 * File:   clist.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 10 mars 2016, 21:57
 * 
 * List à la Lisp
 * 
 * It's not easy to create Lisp like lists without a garbage collector.
 * 
 * We must have a *cons* with a car and a cdr. I didn't do like that,
 * I don't use a garbage collector. As a result, functions like cl_reverse
 * and cl_list_free are dangerous. cl_reverse returns a new list with values 
 * are shared with the original list. When freeing both lists, we must
 * be aware that we can free the values 2 times, which has some negative effects 
 * on the program. At least, Valgrind will complain about 138 allocs against
 * 194 frees. To resolve this problem, each values must have a at least 
 * a reference counter.
 *
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


#ifndef CLIST_H
#define	CLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _TScl_element {
	void *value;

	struct _TScl_element *next;
} TScl_element;

static inline TScl_element *cl_elt_new(void *value) {
	TScl_element *elt = (TScl_element *)calloc(1, sizeof(TScl_element));

	elt->value = value;
	elt->next = NULL;
    return elt;
}

typedef struct _TScl_list {
	TScl_element *first;
} TScl_list;

static inline TScl_list *cl_list_new(void) {
	TScl_list *list = (TScl_list *)calloc(1, sizeof(TScl_list));
	return list;
}

static inline void cl_list_add(TScl_list *list, TScl_element *elt) {
    elt->next = list->first;
    list->first = elt;
}

TScl_list *cl_reverse(TScl_list *list);
void cl_list_for_each(TScl_list *list, void (*on_element)(TScl_element *elt));
void cl_list_free(TScl_list *list, void (*free_value)(void *value));

#endif	/* CLIST_H */

