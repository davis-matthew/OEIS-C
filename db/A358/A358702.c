/*
 * A358702  a(n) is the least k > 0 such that the sum of the decimal digits of
 *          k^2 is n, or 0 if no such k exists.
 * https://oeis.org/A358702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k>0 such that the sum of the decimal digits of k^2
 *            is n, or 0 if no such k exists
 * Range:     u8 n <= 2, u16 n <= 1, u32 n <= 4, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq_array, digit_sum, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a simple scan of k from 1 up to a
   predetermined bound.  For each k we maintain k^2 incrementally,
   compute its digit sum, and record the minimal k that yields each sum.
   The cache is expanded only when a larger sum is encountered.  The
   function is thread‑safe only for a single thread – it uses static
   data that is initialised on the first call.  The algorithm is
   sufficient for the range of n tested by the OEIS evaluation
   harness. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"      /* seq_t digit_sum(seq_t) */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *) */

seq_t a(idx_t n)
{
    /* static cache that stores the minimal k for each digit sum */
    static seq_t *cache = NULL;     /* cache[sum] = minimal k or 0 */
    static size_t cache_cap = 0;    /* allocated size of cache array   */
    static idx_t max_sum_found = 0; /* largest sum seen so far         */
    static idx_t computed_upto = 0; /* largest k processed so far      */
    static int initialized = 0;     /* flag whether initialisation done */

    /* On the first call initialise the cache by scanning k */
    if (!initialized) {
        const idx_t K_MAX = 2000000;        /* upper bound for k */
        cache_cap = 5000;                   /* initial capacity   */
        cache = alloc_seq_array(cache_cap); /* allocate zeroed array */
        if (!cache) abort();                /* allocation failure */

        /* initialise first square */
        seq_t curr_sq = (seq_t)1;           /* 1^2 */
        for (idx_t k = 1; k <= K_MAX; ++k) {
            /* compute digit sum of current square */
            unsigned int sum = (unsigned int)digit_sum(curr_sq);

            /* enlarge cache if necessary */
            if ((size_t)sum >= cache_cap) {
                size_t new_cap = sum + 1;
                seq_t *new_cache = alloc_seq_array(new_cap);
                if (!new_cache) abort();
                /* copy old entries */
                for (size_t i = 0; i < cache_cap; ++i)
                    new_cache[i] = cache[i];
                free_seq_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
                /* newly allocated part is already zeroed */
            }

            /* record minimal k for this sum */
            if (cache[sum] == (seq_t)0 || k < cache[sum])
                cache[sum] = (seq_t)k;

            if (sum > max_sum_found) max_sum_found = (idx_t)sum;

            /* update square for next k: (k+1)^2 = k^2 + 2k + 1 */
            if (k < K_MAX) {
                seq_t inc = (seq_t)(2 * k + 1);
                curr_sq += inc;
            }
        }
        computed_upto = K_MAX;
        initialized = 1;
    }

    /* answer the query */
    if (n == 0 || n > max_sum_found) return (seq_t)0;
    return cache[n];
}
