/*
 * A319689  Number of distinct values of A078898 that occur when map x ->
 *          A064989(x) is iterated, starting from x = n, until an even number
 *          is reached; a(1) = 0 by convention.
 * https://oeis.org/A319689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of A078898(x) for x in the
 *            orbit n, A064989(n), A064989(A064989(n)), ... stopped at the
 *            first even term (inclusive); a(1) = 0. Computed with a sieve
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8992 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A319689_cnt = NULL;  /* A078898 values */
static uint64_t *A319689_f = NULL;    /* A064989 values */
static uint64_t A319689_cap = 0;

static void A319689_build(uint64_t N)
{
    uint64_t *spf = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *pp = (uint64_t *)calloc(N + 1, sizeof(uint64_t)); /* prev prime of prime p */
    uint64_t *pc = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t i, j, last = 1;
    if (!spf || !pp || !pc) abort();
    free(A319689_cnt);
    free(A319689_f);
    A319689_cnt = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    A319689_f = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    if (!A319689_cnt || !A319689_f) abort();
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (i = 2; i <= N; i++) {
        if (spf[i] == i) { pp[i] = last; last = i; }
    }
    A319689_cnt[0] = 0;
    if (N >= 1) { A319689_cnt[1] = 1; A319689_f[1] = 1; }
    for (i = 2; i <= N; i++) {
        uint64_t p = spf[i];
        pc[p]++;
        A319689_cnt[i] = pc[p];
        A319689_f[i] = pp[p] * A319689_f[i / p];
    }
    free(spf);
    free(pp);
    free(pc);
    A319689_cap = N;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t vals[256];
    int len = 0, k, m, dup;
    uint64_t x;
    if (N <= 1) return (seq_t)0;
    if (N > A319689_cap) {
        uint64_t nc = A319689_cap * 2;
        if (nc < N) nc = N;
        if (nc < 1024) nc = 1024;
        A319689_build(nc);
    }
    x = N;
    for (;;) {
        uint64_t v = A319689_cnt[x];
        dup = 0;
        for (m = 0; m < len; m++)
            if (vals[m] == v) { dup = 1; break; }
        if (!dup && len < 256) vals[len++] = v;
        if ((x & 1) == 0) break;
        x = A319689_f[x];
    }
    k = len;
    return (seq_t)k;
}
