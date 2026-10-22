/*
 * A365969  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+1) / (5*k+1) ).
 * https://oeis.org/A365969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365969
   Recurrence (for n >= 5):
       a(n) = a(n-1) + (n-1)*(n-2)*(n-3)*(n-4)*(n-5) * a(n-5)
   Initial values:
       a(0) = a(1) = a(2) = a(3) = a(4) = 1
   The function caches computed values to work in O(n) total time
   per maximal argument. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;           /* greatest n computed so far */

    /* Expand cache if necessary */
    if (!cache || n > cached_n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)                /* out‑of‑memory */
            return (seq_t)0;

        /* Copy old values, if any, and free old array */
        if (cache) {
            for (idx_t i = 0; i <= cached_n; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Compute missing values */
        for (idx_t i = (cache ? cached_n + 1 : 0); i <= n; ++i) {
            if (i < 5) {
                new_cache[i] = (seq_t)1;
            } else {
                /* product (i-1)*(i-2)*(i-3)*(i-4)*(i-5) */
                seq_t prod = (seq_t)(i - 1);
                prod *= (seq_t)(i - 2);
                prod *= (seq_t)(i - 3);
                prod *= (seq_t)(i - 4);
                prod *= (seq_t)(i - 5);

                new_cache[i] = new_cache[i - 1] + prod * new_cache[i - 5];
            }
        }

        cache = new_cache;
        cached_n = n;
    }

    return cache[n];
}
