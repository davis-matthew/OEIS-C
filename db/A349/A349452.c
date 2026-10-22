/*
 * A349452  Dirichlet inverse of A011782, 2^(n-1).
 * https://oeis.org/A349452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349452: Dirichlet inverse of 2^(n-1)
   Recurrence:  a(1) = 1
                 a(n) = - Σ_{d|n, d<n} 2^((n/d)-1) · a(d)   for n>1   */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* allocated array capacity          */
    static idx_t computed_max = 0; /* largest n already computed */

    /* initialise on first call */
    if (!cache) {
        cap = 2;                  /* at least able to store a[1]        */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)1;    /* a(1) = 1                           */
        computed_max = 1;
    }

    /* expand cache if necessary */
    if ((idx_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        /* iterate proper divisors d of k */
        for (idx_t d = 1; d <= k / d; ++d) {
            if (k % d == 0) {
                idx_t d2 = k / d;           /* paired divisor */

                /* contribution from divisor d  (d < k always holds) */
                sum += cache[d] * pow2(d2 - 1);

                /* contribution from paired divisor d2 if distinct and < k */
                if (d2 != d && d2 < k) {
                    sum += cache[d2] * pow2(d - 1);
                }
            }
        }
        cache[k] = -sum;
    }

    computed_max = n;
    return cache[n];
}
