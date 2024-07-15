/*
 * main.c
 *
 * labo-fbsd: experimenting for FreeBSD
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>

#define ONE_MB (1024 * 1024)

typedef struct {
    long phys;      // physical memory, in bytes
    long free;      // free memory, in bytes
    long user;      // user mem i.E. not wired
} TMemInfos;

void die(const char *format, ...);
TMemInfos *new_meminfos(void);
void refresh_meminfos(TMemInfos *mi);

static long get_user_mem(void) {
    int mib[2];
    long phys_mem;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_USERMEM;
    len = sizeof(phys_mem);
    sysctl(mib, 2, &phys_mem, &len, NULL, 0);
    return phys_mem / ONE_MB;
}

static long get_phys_mem(void) {
    int mib[2];
    long phys_mem;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_PHYSMEM;
    len = sizeof(phys_mem);
    sysctl(mib, 2, &phys_mem, &len, NULL, 0);
    return phys_mem / ONE_MB;
}

static long get_free_mem(void) {
    int mib[2];
    struct vmtotal vmt;
    size_t len;

    mib[0] = CTL_VM;
    mib[1] = VM_TOTAL;
    len = sizeof(vmt);
    sysctl(mib, 2, &vmt, &len, NULL, 0);

    return (
            sysconf(_SC_PAGESIZE) *
            vmt.t_free) /
            ONE_MB;
}

#define MAX_STR_LEN	511
void die(const char *format, ...) {
    static char buffer[MAX_STR_LEN + 1];
    va_list aptr;
    int ret;

    va_start(aptr, format);
    ret = vsnprintf(buffer, MAX_STR_LEN, format, aptr);
    va_end(aptr);
    buffer[MAX_STR_LEN] = 0;
    if (ret > 0) {
        fprintf(stderr, "ERROR: %s\n", buffer);
    } else {
        fprintf(stderr, "UNKNOWN ERROR\n");
    }
    exit(1);
}

TMemInfos *new_meminfos(void) {
    TMemInfos *minfos = (TMemInfos *)calloc(1, sizeof(TMemInfos));

    if (minfos == NULL) {
        fprintf(stderr, "Not enough memory\n");
        exit(1);
    }
    minfos->phys = get_phys_mem();
    return minfos;
}

void refresh_meminfos(TMemInfos *mi) {
    mi->free = get_free_mem();
    mi->user = get_user_mem();
}

int main(void) {
    TMemInfos *mi = new_meminfos();
    fprintf(stdout, "%6s - %6s - %6s - %6s - %6s\n",
            "Total", "Free", "Used", "User", "Used?");
    while(1) {
        refresh_meminfos(mi);
        fprintf(stdout, "%6ld - %6ld - %6ld - %6ld - %6ld\n", 
                mi->phys, mi->free, mi->phys-mi->free, 
                mi->user, mi->phys - mi->user);
        sleep(1);
    }
    return 0;
}
