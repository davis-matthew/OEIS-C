/*
 * A351274  a(0) = 1; thereafter a(n) = Sum_{k=1..n} (2*k)^k * Stirling1(n,k).
 * https://oeis.org/A351274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (2*k)^k * s(n,k) where s(n,k) is the signed
 *            Stirling number of the first kind. a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* static caches for memoisation */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)      */
    static seq_t *pow_cache = NULL; /* pow_cache[k] = (2k)^k */
    static seq_t *prev = NULL;      /* Stirling row n-1      */
    static seq_t *curr = NULL;      /* Stirling row n        */

    static idx_t cache_size = 0;    /* size of a_cache      */
    static idx_t pow_size   = 0;    /* size of pow_cache    */
    static idx_t row_size   = 0;    /* max n+1 of Stirling arrays */
    static idx_t computed_max = 0;  /* largest n already computed */

    /* initialise storage on first call */
    if (a_cache == NULL) {
        a_cache   = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;          /* a(0) = 1               */
        cache_size = 1;

        pow_cache = (seq_t *)malloc(sizeof(seq_t));
        pow_cache[0] = (seq_t)1;        /* unused placeholder     */
        pow_size   = 1;

        prev = (seq_t *)malloc(sizeof(seq_t));
        prev[0] = (seq_t)1;            /* S1(0,0) = 1             */
        curr = (seq_t *)malloc(sizeof(seq_t));
        curr[0] = (seq_t)0;
        row_size   = 1;

        computed_max = 0;
    }

    /* extend caches if needed */
    if (n > computed_max) {
        /* grow pow_cache to contain (2*k)^k for k=1..n */
        if (pow_size <= n) {
            size_t new_size = (size_t)(n + 1);
            pow_cache = (seq_t *)realloc(pow_cache, new_size * sizeof(seq_t));
            for (size_t k = pow_size; k < new_size; ++k) {
                seq_t base = (seq_t)(2 * k);
                seq_t exp  = (seq_t)k;
                pow_cache[k] = big_pow(base, exp);
            }
            pow_size = new_size;
        }

        /* grow Stirling arrays if necessary */
        if (row_size <= n) {
            size_t new_size = (size_t)(n + 1);
            prev = (seq_t *)realloc(prev, new_size * sizeof(seq_t));
            curr = (seq_t *)realloc(curr, new_size * sizeof(seq_t));
            for (size_t i = row_size; i < new_size; ++i) {
                prev[i] = (seq_t)0;
                curr[i] = (seq_t)0;
            }
            row_size = new_size;
        }

        /* grow a_cache */
        if (cache_size <= n) {
            cache_size = (size_t)(n + 1);
            a_cache = (seq_t *)realloc(a_cache, cache_size * sizeof(seq_t));
        }

        /* compute a(i) for each i up to n */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            curr[0] = (seq_t)0; /* S1(i,0) = 0 for i>0 */

            /* Stirling recurrence: S1(i,k) = S1(i-1,k-1) + (i-1)*S1(i-1,k) */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t part1 = prev[k - 1];
                seq_t part2 = prev[k] * (seq_t)(i - 1);
                curr[k] = part1 + part2;
            }

            /* compute a(i) from signed Stirling numbers:
               sign = (-1)^{i-k} = +1 if (i-k) even, -1 otherwise */
            seq_t pos_sum = (seq_t)0;
            seq_t neg_sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t prod = pow_cache[k] * curr[k];
                if (((i - k) & 1) == 0)
                    pos_sum = pos_sum + prod;
                else
                    neg_sum = neg_sum + prod;
            }
            a_cache[i] = pos_sum - neg_sum; /* guaranteed non‑negative */

            /* swap prev and curr for next iteration */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        computed_max = n;
    }

    return a_cache[n];
}
