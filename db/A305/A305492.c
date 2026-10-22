/*
 * A305492  a(n) = ((1 + y)^n - (1 - y)^n)/y with y = sqrt(8).
 * https://oeis.org/A305492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Include utility routines for dynamic arrays */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated capacity in seq_t elements */
    static idx_t  cache_len = 0;   /* number of elements currently stored */

    /* initialise cache if needed */
    if (cache_len == 0) {
        cache_cap = 2;                      /* enough for a(0) and a(1) */
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)(n + 1));
        cache_cap = n + 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)7 * cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
