/*
 * A363613  Expansion of Sum_{k>0} x^(2*k)/(1+x^k)^5.
 * https://oeis.org/A363613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d+2,4)
 * Range:     i8 n <= 7, i16 n <= 29, i32 n <= 466, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9230 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute (d+2 choose 4) as (d+2)(d+1)d(d-1)/24.
   All arithmetic is performed in type seq_t to avoid overflow. */
static inline seq_t binom_d_plus2_choose4(idx_t d)
{
    seq_t t1 = (seq_t)(d + 2);
    seq_t t2 = (seq_t)(d + 1);
    seq_t t3 = (seq_t)d;
    seq_t t4 = (seq_t)(d - 1);
    return t1 * t2 * t3 * t4 / 24;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* 1‑based cache; cache[0] unused */
    static idx_t cache_size = 0;     /* maximum index for which cache was built */

    if (n < 1)           /* undefined for zero or negative indices */
        return 0;

    /* Recompute cache up to n if necessary. */
    if (n > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = mem_new(seq_t, new_size);
        if (!new_cache)   /* allocation failure */
            return 0;

        /* For each divisor d, add its contribution to all multiples m. */
        for (idx_t d = 1; d <= n; ++d) {
            seq_t contrib = binom_d_plus2_choose4(d);
            if (d & 1)          /* odd d -> sign = -1 */
                contrib = -contrib;
            for (idx_t m = d; m <= n; m += d)
                new_cache[m] += contrib;
        }

        /* Replace old cache. */
        mem_free(cache);
        cache       = new_cache;
        cache_size  = n;
    }

    return cache[n];
}
