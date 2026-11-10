/*
 * A375599  The n-th record of A294336 occur at index 2^a(n).
 * https://oeis.org/A375599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index m >= 0 at which A294337(m) sets a new record
 *            (A294337(0)=1); A294337(m) = Sum_{d|m} A294336(d), A294336(1)=1,
 *            A294336(k) = Sum_{t | A052409(k)} A294336(t); computed by a
 *            sieve with a growing limit
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t *rec_list = NULL;
static idx_t rec_cnt = 0;
static idx_t rec_limit = 0;

static idx_t A294599_gcd(idx_t x, idx_t y) {
    while (y) { idx_t t = x % y; x = y; y = t; }
    return x;
}

static void A375599_build(idx_t M) {
    idx_t *spf = (idx_t *)malloc((M + 1) * sizeof(idx_t));
    seq_t *A = mem_new(seq_t, (M + 1));
    seq_t *f = mem_new(seq_t, (M + 1));
    if (!spf || !A || !f) { if (spf) free(spf); if (A) mem_free(A); if (f) mem_free(f); return; }
    for (idx_t i = 0; i <= M; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= M; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= M; j += i)
                if (spf[j] == j) spf[j] = i;
    A[1] = (seq_t)1;
    for (idx_t i = 2; i <= M; ++i) {
        idx_t t = i, g = 0;
        while (t > 1) {
            idx_t p = spf[t], e = 0;
            while (t % p == 0) { t /= p; ++e; }
            g = g ? A294599_gcd(g, e) : e;
        }
        seq_t s = 0;
        for (idx_t d = 1; d <= g; ++d)
            if (g % d == 0) s += A[d];
        A[i] = s;
    }
    for (idx_t d = 1; d <= M; ++d)
        for (idx_t m = d; m <= M; m += d)
            f[m] += A[d];

    if (rec_list) free(rec_list);
    rec_list = (idx_t *)malloc((M + 2) * sizeof(idx_t));
    rec_cnt = 0;
    rec_list[rec_cnt++] = 0;
    seq_t best = 1;
    for (idx_t m = 1; m <= M; ++m) {
        if (f[m] > best) { best = f[m]; rec_list[rec_cnt++] = m; }
    }
    rec_limit = M;
    free(spf);
    mem_free(A);
    mem_free(f);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (rec_limit == 0) A375599_build((idx_t)1 << 12);
    while (rec_cnt < n) {
        idx_t old = rec_limit;
        A375599_build(old * 4);
        if (rec_limit == old) return (seq_t)0;
    }
    return (seq_t)rec_list[n - 1];
}
