/*
 * A305805  Dirichlet inverse of A175851.
 * https://oeis.org/A305805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = -Sum_{d|n, d<n} A175851(n/d)*a(d),
 *            where A175851(m) = 1 for m=1 or prime, else m - previousprime(m)
 *            + 1 (divisor sieve, memoized)
 * Range:     i8 n <= 779, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9787 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A305805_tab = NULL;
static idx_t A305805_cap = 0;

static void A305805_build(idx_t N)
{
    size_t sz = (size_t)N + 1;
    uint8_t *comp = (uint8_t*)calloc(sz, 1);
    if (!comp) abort();
    for (idx_t p = 2; p * p <= N; ++p)
        if (!comp[p])
            for (idx_t m = p * p; m <= N; m += p) comp[m] = 1;

    seq_t *f = mem_new(seq_t, sz);
    seq_t *acc = mem_new(seq_t, sz);
    seq_t *res = mem_new(seq_t, sz);

    idx_t prev = 1;
    f[1] = 1;
    for (idx_t m = 2; m <= N; ++m) {
        if (!comp[m]) { f[m] = 1; prev = m; }
        else f[m] = (seq_t)m - (seq_t)prev + 1;
    }

    for (idx_t d = 1; d <= N; ++d) {
        if (d == 1) res[d] = 1;
        else res[d] = -acc[d];
        if (res[d] != 0) {
            idx_t k = 2;
            for (idx_t m = 2 * d; m <= N; m += d, ++k)
                acc[m] += f[k] * res[d];
        }
    }

    free(comp);
    mem_free(f);
    mem_free(acc);
    if (A305805_tab) mem_free(A305805_tab);
    A305805_tab = res;
    A305805_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A305805_cap) {
        idx_t N = A305805_cap * 2;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A305805_build(N);
    }
    return A305805_tab[n];
}
