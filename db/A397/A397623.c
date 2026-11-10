/*
 * A397623  a(n) = prime(A005652(n)).
 * https://oeis.org/A397623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(A005652(n)), where A005652(n) is the n-th k with k
 *            + floor(sqrt(5*k^2)) odd (search), and prime(m) is found by a
 *            sieve
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static uint64_t *a397623_pr = NULL;
static uint64_t a397623_cnt = 0;
static uint64_t a397623_lim = 0;

static void a397623_ensure(uint64_t m) {
    while (a397623_cnt < m) {
        uint64_t lim = a397623_lim < 64 ? 64 : a397623_lim * 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j, c = 0;
        if (!comp) return;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 2; i <= lim; i++) if (!comp[i]) c++;
        uint64_t *p = (uint64_t *)malloc((size_t)c * sizeof(uint64_t));
        if (!p) { free(comp); return; }
        c = 0;
        for (i = 2; i <= lim; i++) if (!comp[i]) p[c++] = i;
        free(comp);
        free(a397623_pr);
        a397623_pr = p;
        a397623_cnt = c;
        a397623_lim = lim;
    }
}

static uint64_t *a397623_ks = NULL;
static uint64_t a397623_klen = 0;
static uint64_t a397623_kcap = 0;

static uint64_t a397623_kth(uint64_t n) {
    if (n > a397623_klen) {
        if (n > a397623_kcap) {
            uint64_t nc = a397623_kcap ? a397623_kcap : 64;
            while (nc < n) nc *= 2;
            a397623_ks = (uint64_t *)realloc(a397623_ks, sizeof(uint64_t) * nc);
            a397623_kcap = nc;
        }
        uint64_t k = a397623_klen ? a397623_ks[a397623_klen - 1] : 0;
        while (a397623_klen < n) {
            k++;
            if (((k + isqrt_u64(5 * k * k)) & 1) == 1)
                a397623_ks[a397623_klen++] = k;
        }
    }
    return a397623_ks[n - 1];
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    uint64_t k = a397623_kth((uint64_t)n);
    a397623_ensure(k);
    if (a397623_cnt < k) return (seq_t)0;
    return (seq_t)a397623_pr[k - 1];
}
