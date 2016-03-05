/*
 * File:   solar-infos.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 4 mars 2016, 23:21
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
#include <stdbool.h>
#include <unistd.h>
#include <sys/loadavg.h>
#include <pthread.h>

#include "solar-infos.h"

#define ONE_MB (1024 * 1024)

static bool soli_bstop = false;
static pthread_t soli_thread;

static TSsysconf current_sysconf = {
    .inited = false
};

TSsysconf *fill_soli_sysconf(void) {
    time_t tp;

    time(&tp);
    current_sysconf.tm = localtime(&tp);
    if (!current_sysconf.inited) {
        current_sysconf.num_procs = sysconf(_SC_NPROCESSORS_CONF);
        current_sysconf.page_size = sysconf(_SC_PAGESIZE);
        current_sysconf.num_pages = sysconf(_SC_PHYS_PAGES);
        current_sysconf.inited = true;
    }
    current_sysconf.procs_online = sysconf(_SC_NPROCESSORS_ONLN);
    current_sysconf.free_pages = sysconf(_SC_AVPHYS_PAGES);

    current_sysconf.mem = (longlong_t) ((longlong_t) current_sysconf.num_pages * (longlong_t) current_sysconf.page_size);
    current_sysconf.mem /= ONE_MB;
    current_sysconf.free_mem = (longlong_t) current_sysconf.free_pages * (longlong_t) current_sysconf.page_size;
    current_sysconf.free_mem /= ONE_MB;
	if (getloadavg (current_sysconf.load_av, 3) == -1) {
		for (int i=0; i<3; i++) {
			current_sysconf.load_av[i] = -1.0;
		}
	}
	return &current_sysconf;
}

TSsysconf *soli_sysconf(void) {
	return &current_sysconf;
}

static void *soli_loop(void *arg) {
	while (!soli_bstop) {
		fprintf(stdout, "soli_loop\n");
		fill_soli_sysconf();
		sleep(1);
	}
	fprintf(stdout, "soli_loop OUT\n");
	return NULL;
}

void soli_start(void) {
	fill_soli_sysconf();
	pthread_create (&soli_thread, NULL, soli_loop, NULL);
	soli_bstop = false;
}

void soli_stop(void) {
	fprintf(stdout, "soli_stop...\n");
	soli_bstop = true;
	pthread_join(&soli_thread, NULL);
	fprintf(stdout, "soli_stop OK.\n");
}