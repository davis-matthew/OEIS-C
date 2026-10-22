/*
 * A381275  Expansion of e.g.f. exp(x * cos(2*x)).
 * https://oeis.org/A381275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A381275: expansion of the e.g.f. exp(x * cos(2*x))
   Formula (recurrence):  a(0) = 1
   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (-4)^k · (2k+1) · binomial(n-1,2k) · a(n-2k-1)
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored in cache */

    /* grow the cache so that it contains up to index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;           /* inclusive */
        /* allocate a larger array */
        seq_t *new_cache = alloc_seq_array((size_t)new_size);

        /* copy old values to the new array */
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }

        /* free the old cache if it existed */
        if (cache) {
            free(cache);
        }

        /* assign the new cache and update size */
        cache       = new_cache;
        idx_t old_size = cache_size;
        cache_size   = new_size;

        /* initialise base cases if they have not been set yet */
        if (old_size == 0) {              /* a(0) */
            cache[0] = (seq_t)1;
            old_size = 1;
        }
        if (old_size <= 1 && new_size > 1) { /* a(1) */
            cache[1] = (seq_t)1;
            old_size = 2;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = old_size; i < new_size; ++i) {
            idx_t limit = (i - 1) / 2;
            seq_t sum = (seq_t)0;
            seq_t powk = (seq_t)1; /* (-4)^k, starts with k=0 */

            for (idx_t k = 0; k <= limit; ++k) {
                /* binomial coefficient binomial(i-1, 2*k) */
                seq_t bin = binomial(i - 1, 2 * k);
                /* a(i-2*k-1) */
                seq_t a_term = cache[i - 2 * k - 1];
                /* (2*k+1) factor */
                seq_t two_k_plus1 = (seq_t)(2 * k + 1);

                seq_t term = bin * a_term;
                term = term * two_k_plus1;
                term = term * powk;

                sum += term;

                /* prepare next power of (-4) */
                powk = powk * (seq_t)(-4);
            }

            cache[i] = sum;
        }
    }

    return cache[n];
}
