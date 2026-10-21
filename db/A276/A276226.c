/*
 * A276226  a(n+3) = 2*a(n+2) + a(n+1) + a(n) with a(0)=0, a(1)=6, a(2)=8.
 * https://oeis.org/A276226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 2*a(n+2) + a(n+1) + a(n)
   Initial values: a(0) = 0, a(1) = 6, a(2) = 8 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoization (supports out-of-order calls) */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;      /* allocated size of cache array     */
    static idx_t    last  = (idx_t)-1; /* highest index cached so far     */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 8;
        cache = seq_alloc_new_array(cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)8;
        last    = 2;
    }

    /* enlarge cache if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) new_arr[i] = cache[i];
        free_cache(cache);
        cache = new_arr;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (last < (idx_t)n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1] + cache[i-2] + cache[i-3];
        }
        last = n;
    }

    return cache[n];
}
