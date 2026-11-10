/*
 * A358476  Number of 3's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3's among the terms appended to the first list
 *            in step n-1 of the A030717 construction; a(1) = 0 (initial list
 *            [1]). Computed by direct simulation
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static unsigned long long *A358476_cnt = NULL;
static size_t A358476_cap = 0;
static size_t A358476_maxv = 0;
static unsigned long long *A358476_tmp = NULL;
static size_t A358476_tmpcap = 0;
static unsigned long long *A358476_res = NULL;
static size_t A358476_nres = 0;
static size_t A358476_rescap = 0;

static void A358476_grow_cnt(size_t need)
{
    size_t nc = A358476_cap ? A358476_cap : 64;
    while (nc <= need) nc <<= 1;
    unsigned long long *p = (unsigned long long *)calloc(nc, sizeof(unsigned long long));
    for (size_t i = 0; i < A358476_cap; ++i) p[i] = A358476_cnt[i];
    free(A358476_cnt);
    A358476_cnt = p;
    A358476_cap = nc;
}

static void A358476_step(void)
{
    size_t k = 0;
    if (A358476_tmpcap < A358476_maxv + 1) {
        free(A358476_tmp);
        A358476_tmpcap = (A358476_maxv + 1) * 2;
        A358476_tmp = (unsigned long long *)malloc(A358476_tmpcap * sizeof(unsigned long long));
    }
    for (size_t v = 1; v <= A358476_maxv; ++v) {
        if (A358476_cnt[v] > 0) A358476_tmp[k++] = A358476_cnt[v];
    }
    unsigned long long c3 = 0;
    for (size_t i = 0; i < k; ++i) {
        unsigned long long f = A358476_tmp[i];
        if (f >= A358476_cap) A358476_grow_cnt((size_t)f);
        A358476_cnt[f]++;
        if ((size_t)f > A358476_maxv) A358476_maxv = (size_t)f;
        if (f == 3) c3++;
    }
    if (A358476_nres >= A358476_rescap) {
        A358476_rescap = A358476_rescap ? A358476_rescap * 2 : 256;
        A358476_res = (unsigned long long *)realloc(A358476_res, A358476_rescap * sizeof(unsigned long long));
    }
    A358476_res[A358476_nres++] = c3;
}

seq_t a(idx_t n)
{
    if (A358476_cnt == NULL) {
        A358476_grow_cnt(64);
        A358476_cnt[1] = 1;
        A358476_maxv = 1;
    }
    if (n <= 1) return (seq_t)0;
    while (A358476_nres < (size_t)(n - 1)) A358476_step();
    return (seq_t)A358476_res[n - 2];
}
