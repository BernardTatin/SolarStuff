/*
 * freebsd.h
 */

#if !defined(_FREEBSD_H__)

#define ONE_KB (1024)
#define ONE_MB (1024 * 1024)

/*
struct vmtotal {
	uint64_t	t_vm;		// total virtual memory
	uint64_t	t_avm;		// active virtual memory
	uint64_t	t_rm;		// total real memory in use
	uint64_t	t_arm;		// active real memory
	uint64_t	t_vmshr;	// shared virtual memory
	uint64_t	t_avmshr;	// active shared virtual memory
	uint64_t	t_rmshr;	// shared real memory
	uint64_t	t_armshr;	// active shared real memory
	uint64_t	t_free;		// free memory pages
	int16_t		t_rq;		// length of the run queue
	int16_t		t_dw;		// threads in ``disk wait'' (neg priority)
	int16_t		t_pw;		// threads in page wait
	int16_t		t_sl;		// threads sleeping in core
	int16_t		t_sw;		// swapped out runnable/short block threads
	uint16_t	t_pad[3];
};
 */
typedef struct {
	uint64_t	t_vm;		// total virtual memory
	uint64_t	t_avm;		// active virtual memory
	uint64_t	t_rm;		// total real memory in use
	uint64_t	t_arm;		// active real memory
	uint64_t	t_vmshr;	// shared virtual memory
	uint64_t	t_avmshr;	// active shared virtual memory
	uint64_t	t_rmshr;	// shared real memory
	uint64_t	t_armshr;	// active shared real memory
	uint64_t	t_free;		// free memory pages
	int16_t		t_rq;		// length of the run queue
	int16_t		t_dw;		// threads in ``disk wait'' (neg priority)
    int16_t		t_pw;		// threads in page wait
	int16_t		t_sl;		// threads sleeping in core
	int16_t		t_sw;		// swapped out runnable/short block threads
	uint16_t	t_pad[3];
                    //
    long phys;      // physical memory, in bytes
    long free;      // free memory, in bytes
    long user;      // user mem i.E. not wired
} TMemInfos;

static inline long get_long_MIB(const int levl1, const int levl2) {
    int mib[2];
    long value;
    size_t len;

    mib[0] = levl1;
    mib[1] = levl2;
    len = sizeof(value);
    sysctl(mib, 2, &value, &len, NULL, 0);
    return value;
}

static inline bool get_sysctl_struct(void *data, size_t data_len,
        const int levl1, const int levl2) {
    int mib[2];

    mib[0] = levl1;
    mib[1] = levl2;
    // int	sysctl(const int *, unsigned int, void *, size_t *, const void *, size_t);
    sysctl(mib, 2, data, &data_len, NULL, 0);
    return true;
}

static inline long get_user_mem(void) {
    return get_long_MIB(CTL_HW, HW_USERMEM) / ONE_MB;
}

static inline long get_phys_mem(void) {
    return get_long_MIB(CTL_HW, HW_PHYSMEM) / ONE_MB;
}

#endif // _FREEBSD_H__
