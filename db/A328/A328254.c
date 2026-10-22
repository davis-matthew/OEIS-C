/*
 * A328254  Dirichlet g.f.: 1 / (zeta(s) * zeta(s-1) * zeta(s-2)).
 * https://oeis.org/A328254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e a(p^e) =
 *            -(p^2+p+1) if e = 1, p^3+p^2+p if e = 2, -p^3 if e = 3, 0
 *            otherwise. a(1) = 1
 * Range:     i8 n <= 9, i16 n <= 113, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_a_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_a_n.h"

/* Memoised sequence. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = (idx_t)0;
    static idx_t cache_max = (idx_t)0;

    /* Grow the cache if necessary. */
    if (n + (idx_t)1 > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            sizeof(seq_t) * (n + (idx_t)1));
        if (!new_cache) return (seq_t)0; /* allocation failed (unlikely) */
        cache = new_cache;
        cache_cap = n + (idx_t)1;
    }

    /* Compute and store missing values. */
    if (n > cache_max) {
        for (idx_t i = cache_max + (idx_t)1; i <= n; ++i) {
            cache[i] = compute_a_n(i);
        }
        cache_max = n;
    }

    return cache[n];
}
