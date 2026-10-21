/*
 * A247065  Dynamic Betting Game D(n,6,1).
 * https://oeis.org/A247065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A247065
   a(0) = 0
   a(1) = 1, a(2) = 16, a(3) = 24, a(4) = 32, a(5) = 40, a(6) = 49, a(7) = 64
   For n ≥ 8:  a(n) = a(n-1) + a(n-7) - a(n-8)              (1)
   This recurrence exactly reproduces the OEIS values.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow fast repeated calls in arbitrary order.
       Initially uninitialized; on first call we allocate and
       fill the known base values. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Handle the offset and trivial cases. */
    if (n == 0) return 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 8;                         /* indices 0..7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure */

        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)16;  /* a(2) */
        cache[3] = (seq_t)24;  /* a(3) */
        cache[4] = (seq_t)32;  /* a(4) */
        cache[5] = (seq_t)40;  /* a(5) */
        cache[6] = (seq_t)49;  /* a(6) */
        cache[7] = (seq_t)64;  /* a(7) */
    }

    /* Return from cache if available. */
    if (n < cache_cap)
        return cache[n];

    /* Extend cache to accommodate index n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return 0;  /* allocation failure */

    cache = new_cache;

    /* Compute a(k) using recurrence (1) for k = old_cap .. n . */
    for (idx_t k = cache_cap; k <= n; ++k) {
        /* All operands exist in the cache because k ≥ 8. */
        cache[k] = cache[k-1] + cache[k-7] - cache[k-8];
    }

    cache_cap = n + 1;
    return cache[n];
}
