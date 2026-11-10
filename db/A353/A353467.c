/*
 * A353467  a(1) = 1, for n > 1, a(n) = -Sum_{d|n, d<n} a(A252463(n/d)) *
 *          a(d).
 * https://oeis.org/A353467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} a(A252463(n/d)) * a(d), n > 1
 *            (A252463 via sieve; memoized table)
 * Range:     i8 n <= 63, i16 n <= 1151, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9813 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *a353467_tab = NULL;
static idx_t a353467_N = 0;

static void a353467_build(idx_t N)
{
    idx_t i, j, k;
    idx_t *spf = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    idx_t *pp = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    idx_t *f = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    seq_t *t;
    idx_t last;
    if (!spf || !pp || !f) { free(spf); free(pp); free(f); return; }
    for (i = 0; i <= N; i++) { spf[i] = 0; pp[i] = 0; }
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    last = 1;
    for (i = 2; i <= N; i++)
        if (spf[i] == i) { pp[i] = last; last = i; }
    f[0] = 0;
    if (N >= 1) f[1] = 1;
    for (i = 2; i <= N; i++) {
        if ((i & 1u) == 0u) f[i] = i >> 1;
        else {
            idx_t p = spf[i];
            f[i] = f[i / p] * pp[p];
        }
    }
    t = mem_new(seq_t, (size_t)(N + 1));
    t[0] = (seq_t)0;
    if (N >= 1) t[1] = (seq_t)1;
    for (i = 2; i <= N; i++) {
        seq_t s = (seq_t)0;
        for (k = 1; k * k <= i; k++) {
            if (i % k != 0) continue;
            idx_t e = i / k;
            /* divisor d = k, cofactor e */
            s += t[f[e]] * t[k];
            if (e != k && k > 1) {
                /* divisor d = e, cofactor k */
                s += t[f[k]] * t[e];
            }
        }
        t[i] = -s;
    }
    free(spf); free(pp); free(f);
    if (a353467_tab) mem_free(a353467_tab);
    a353467_tab = t;
    a353467_N = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > a353467_N) {
        idx_t N = a353467_N * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a353467_build(N);
    }
    return a353467_tab[n];
}
