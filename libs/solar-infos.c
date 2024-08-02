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
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/utsname.h>

#include "compat.h"


#include "solar-infos.h"

#define ONE_MB (1024 * 1024)

static bool soli_bstop = false;
static pthread_t soli_thread;

static TSsysconf current_sysconf = {
    .inited = false
};

static TSsysconf display_sysconf;
static TSsysconf buffer_sysconf;
static pthread_mutex_t mutex_sysconf = PTHREAD_MUTEX_INITIALIZER;

static LONGLONG get_phys_mem(void) {
#if defined(__FreeBSD__)
    int mib[2];
    long phys_mem;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_PHYSMEM;
    len = sizeof(phys_mem);
    sysctl(mib, 2, &phys_mem, &len, NULL, 0);
    return (LONGLONG)phys_mem / ONE_MB;

#else
    return (LONGLONG)(
            (LONGLONG)sysconf(_SC_PAGESIZE) *
            (LONGLONG)sysconf(_SC_PHYS_PAGES) /
            ONE_MB);
#endif
}

static LONGLONG get_free_mem(void) {
#if defined(__SunOS)
    return (LONGLONG)(
            (LONGLONG)sysconf(_SC_PAGESIZE) *
            (LONGLONG)sysconf(_SC_AVPHYS_PAGES) /
            ONE_MB);
#elif defined(__linux__)
    return (LONGLONG)(
            (LONGLONG)sysconf(_SC_PAGESIZE) *
            (LONGLONG)sysconf(_SC_AVPHYS_PAGES) /
            ONE_MB);
#elif defined(__FreeBSD__)
    int mib[2];
    struct vmtotal vmt;
    size_t len;

    mib[0] = CTL_VM;
    mib[1] = VM_TOTAL;
    len = sizeof(vmt);
    sysctl(mib, 2, &vmt, &len, NULL, 0);

    return (LONGLONG)(
            (LONGLONG)sysconf(_SC_PAGESIZE) *
            (LONGLONG)vmt.t_free /
            ONE_MB);
#endif
}

void fill_staticsoli_sysconf(void) {
    if (!current_sysconf.inited) {
        current_sysconf.num_procs = sysconf(_SC_NPROCESSORS_CONF);
        current_sysconf.page_size = sysconf(_SC_PAGESIZE);
        current_sysconf.num_pages = sysconf(_SC_PHYS_PAGES);
        current_sysconf.mem = get_phys_mem();
        current_sysconf.inited = true;
		current_sysconf.uname_ok = uname(&current_sysconf.sname) != -1;
    }
}

void fill_dynasoli_sysconf(void) {
    static time_t tp;
    static struct tm ltm;

    time(&tp);
    // Valgrind complains about lost bytes here, with localtime as well with localtime_r
    // these functions seems to allocate some memory.
    // (line numbers have changed...)
    // 23,440 bytes in 1 blocks are still reachable in loss record 4 of 4
    //    at 0x484D314: malloc (vg_replace_malloc.c:450)
    //    by 0x4C29CB0: ??? (in /lib/libc.so.7)
    //    by 0x4C29F75: ??? (in /lib/libc.so.7)
    //    by 0x202A8F: fill_dynasoli_sysconf (solar-infos.c:122)
    //    by 0x2027AD: fill_soli_sysconf (solar-infos.h:40)
    //    by 0x202655: create_sysinfo_list (sysinfo-list.c:56)
    //    by 0x202524: main (main.c:91)
    localtime_r(&tp, &ltm);
    current_sysconf.tm = &ltm;
    current_sysconf.procs_online = sysconf(_SC_NPROCESSORS_ONLN);

    current_sysconf.free_mem = get_free_mem();
#if !defined(__linux__)
	if (getloadavg (current_sysconf.load_av, 3) == -1) {
		for (int i=0; i<3; i++) {
			current_sysconf.load_av[i] = -1.0;
		}
	}
#endif
	pthread_mutex_lock(&mutex_sysconf);
	memmove (&display_sysconf, &current_sysconf, sizeof(TSsysconf));
	pthread_mutex_unlock(&mutex_sysconf);
}

TSsysconf *soli_sysconf(void) {
	pthread_mutex_lock(&mutex_sysconf);
	memmove (&buffer_sysconf, &display_sysconf, sizeof(TSsysconf));
	pthread_mutex_unlock(&mutex_sysconf);
	return &buffer_sysconf;
}

static void *soli_loop(void *arg) {
	struct timespec ts;

    if (arg != NULL) {
        fprintf(stderr, "WARNING: soli_loop arg not NULL");
    }
	while (!soli_bstop) {
		fill_dynasoli_sysconf();
		ts.tv_sec = 0;
		ts.tv_nsec = 50000000l;
		nanosleep(&ts, NULL);
	}
	return NULL;
}

void soli_start(void) {
	fill_staticsoli_sysconf();
	fill_dynasoli_sysconf();
	pthread_create (&soli_thread, NULL, soli_loop, NULL);
	soli_bstop = false;
}

void soli_stop(void) {
	soli_bstop = true;
	pthread_join(soli_thread, NULL);
}
