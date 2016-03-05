/* 
 * File:   solar-infos.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 4 mars 2016, 23:18
 */

#ifndef SOLAR_INFOS_H
#define	SOLAR_INFOS_H

typedef struct {
	bool inited;

	long num_procs;
	long procs_online;
	long page_size;
	long num_pages;
	long free_pages;
	longlong_t mem;
	longlong_t free_mem;

	double load_av [3];
    struct tm *tm;

} TSsysconf;

TSsysconf *soli_sysconf(void);

void soli_start(void);
void soli_stop(void);

#endif	/* SOLAR_INFOS_H */

