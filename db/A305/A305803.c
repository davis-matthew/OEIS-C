/*
 * A305803  Dirichlet inverse of A078898.
 * https://oeis.org/A305803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = -Sum_{d|n, d<n} A078898(n/d)*a(d),
 *            where A078898(m) = #{k<=m : lpf(k)=lpf(m)}, A078898(1)=1
 *            (computed by sieve)
 * Range:     i8 n <= 261, i16 n <= 22439, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 8649 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A305803_cache = NULL;
static uint64_t A305803_size = 0;

static void A305803_build(uint64_t N)
{
    uint64_t *spf = mem_new(uint64_t, N + 1);
    uint64_t *f = mem_new(uint64_t, N + 1);
    uint64_t *cnt = mem_new(uint64_t, N + 1);
    seq_t *a = mem_new(seq_t, N + 1);
    uint64_t i, d, m;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (m = i; m <= N; m += i)
                if (spf[m] == 0) spf[m] = i;
        }
    }
    f[1] = 1;
    for (i = 2; i <= N; i++) {
        cnt[spf[i]]++;
        f[i] = cnt[spf[i]];
    }
    /* a[] doubles as accumulator of sum f(m/d)*a(d) */
    for (d = 1; d <= N; d++) {
        if (d == 1) a[1] = 1;
        else a[d] = -a[d];
        for (m = 2 * d; m <= N; m += d)
            a[m] += (seq_t)f[m / d] * a[d];
    }
    mem_free(spf);
    mem_free(f);
    mem_free(cnt);
    if (A305803_cache) mem_free(A305803_cache);
    A305803_cache = a;
    A305803_size = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A305803_size) {
        uint64_t N = A305803_size * 2;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A305803_build(N);
    }
    return A305803_cache[n];
}
