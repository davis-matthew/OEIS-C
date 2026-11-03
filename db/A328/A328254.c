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
 * Validated: standard, pass, 66 terms through n = 9946 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute a single value of the sequence from its multiplicative definition. */
static seq_t compute_a_n(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    for (idx_t p = (idx_t)2; p <= tmp / p; ++p) {
        if (tmp % p == (idx_t)0) {
            idx_t e = (idx_t)0;
            while (tmp % p == (idx_t)0) {
                tmp /= p;
                ++e;
                if (e > (idx_t)3) return (seq_t)0;
            }

            seq_t p_seq = (seq_t)p;
            seq_t p2 = p_seq * p_seq;
            seq_t p3 = p2 * p_seq;
            seq_t part;

            if (e == (idx_t)1)
                part = -(p2 + p_seq + (seq_t)1);
            else if (e == (idx_t)2)
                part = p3 + p2 + p_seq;
            else        /* e == 3 */
                part = -(p3);

            result *= part;
            if (result == (seq_t)0) return (seq_t)0;
        }
    }

    if (tmp > (idx_t)1) {
        seq_t p_seq = (seq_t)tmp;
        seq_t p2 = p_seq * p_seq;
        seq_t part = -(p2 + p_seq + (seq_t)1);
        result *= part;
    }

    return result;
}

/* Memoised sequence. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = (idx_t)0;
    static idx_t cache_max = (idx_t)0;

    /* Grow the cache if necessary. */
    if (n + (idx_t)1 > cache_cap) {
        seq_t *new_cache = mem_resize(seq_t, cache, (n + (idx_t)1));
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
