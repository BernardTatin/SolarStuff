/* 
 * File:   compat.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 8 juillet 2024, 23:15
 */

#ifndef COMPAT_H
#define	COMPAT_H


#if defined(__SunOS)
#include <sys/loadavg.h>

#define LONGLONG	longlong_t

#elif defined(__linux__)
#define LOADAVG_1MIN 0
#define LOADAVG_5MIN 1
#define LOADAVG_15MIN 2

#define	LOADAVG_NSTATS	3

#define LONGLONG	long long
#elif defined(__FreeBSD__)
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>

#define LOADAVG_1MIN 0
#define LOADAVG_5MIN 1
#define LOADAVG_15MIN 2

#define	LOADAVG_NSTATS	3

#define LONGLONG	long long
#endif

#endif	/* COMPAT_H */

