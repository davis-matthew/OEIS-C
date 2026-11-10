/*
 * A301798  Expansion of Product_{k>=1} (1 + x^k)^A002131(k).
 * https://oeis.org/A301798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} (1 + x^k)^A002131(k), with
 *            A002131(k) = Sum_{d|k, k/d odd} d. Each factor (1+x^k)^e is
 *            expanded as Sum_j C(e,j) x^(j*k); the binomials come from
 *            Pascal's rule. Only additions and multiplications are used, so
 *            there is no division
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 50, u64 n <= 128, u128 n <= 338,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 986 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A301798_cache = 0;
static idx_t A301798_len = 0; /* number of valid terms in cache */

seq_t a(idx_t n) {
    if (n < A301798_len) return A301798_cache[n];
    idx_t N = n;
    if (N < 64) N = 64;
    if (N < 2 * A301798_len) N = 2 * A301798_len;
    size_t sz = (size_t)N + 1;

    /* A002131(k) for k <= N as native integers (small: <= sigma(k)) */
    idx_t *c = (idx_t *)malloc(sz * sizeof(idx_t));
    for (idx_t i = 0; i <= N; i++) c[i] = 0;
    for (idx_t d = 1; d <= N; d++)
        for (idx_t m = d; m <= N; m += 2 * d)
            c[m] += d;

    seq_t *p = mem_new(seq_t, sz);
    seq_t *B = mem_new(seq_t, sz);
    p[0] = 1;

    for (idx_t k = 1; k <= N; k++) {
        idx_t e = c[k];
        idx_t m = N / k; /* max j needed */
        /* Build C(e, j) for j = 0..m via Pascal's rule (additions only) */
        B[0] = 1;
        for (idx_t j = 1; j <= m; j++) B[j] = 0;
        for (idx_t t = 0; t < e; t++) {
            idx_t top = (t + 1 < m) ? t + 1 : m;
            for (idx_t j = top; j >= 1; j--) B[j] += B[j - 1];
        }
        /* p *= (1 + x^k)^e, in place, descending */
        for (idx_t i = N; i >= k; i--) {
            seq_t s = 0;
            idx_t jmax = i / k;
            if (jmax > e) jmax = e;
            for (idx_t j = 1; j <= jmax; j++)
                s += B[j] * p[i - j * k];
            p[i] += s;
        }
    }

    free(c);
    mem_free(B);
    if (A301798_cache) mem_free(A301798_cache);
    A301798_cache = p;
    A301798_len = N + 1;
    return A301798_cache[n];
}
