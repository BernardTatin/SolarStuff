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

#include "freebsd.h"

void die(const char *format, ...);
TMemInfos *new_meminfos(void);
void refresh_meminfos(TMemInfos *mi);


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
    get_sysctl_struct(mi, sizeof(struct vmtotal),
            CTL_VM, VM_TOTAL);

}

static void print_vm(TMemInfos *mi) {
    uint64_t *ptr = (uint64_t *)mi;

    for (int i=0; i<9; i++) {
        switch(i) {
            case 0:
            case 1:
                fprintf(stdout, "%6lu mb ", *(ptr++) / ONE_MB);
                break;
            case 8:
                // free memory pages
                fprintf(stdout, "%6lu mb ", *(ptr++) * 4096 / ONE_MB);
                break;
            default:
                fprintf(stdout, "%6lu kb ", *(ptr++) / ONE_KB);
                break;
        }
    }
    fprintf(stdout, "%6ld", mi->free);
    fprintf(stdout, "\n");
}

int main(void) {
    TMemInfos *mi = new_meminfos();
    fprintf(stdout, "%6s - %6s - %6s - %6s - %6s\n",
            "Total", "Free", "Used", "User", "Used?");
   for (int i=0; i<5; i++) {
        refresh_meminfos(mi);
        print_vm(mi);
//         fprintf(stdout, "%6ld - %6ld - %6ld - %6ld - %6ld\n", 
//                 mi->phys, mi->free, mi->phys-mi->free, 
//                 mi->user, mi->phys - mi->user);
        sleep(1);
    }
    free(mi);
    return 0;
}
