/*
 * A298402  a(n) = 2*a(n-1) - a(n-3) + a(floor(n/2)), where a(0) = 1, a(1) =
 *          1, a(2) = 1.
 * https://oeis.org/A298402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(floor(n/2)), a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/alloc_cache.h"      /* seq_t *alloc_cache(idx_t n) */
#include "../utils/free_cache.h"       /* void free_cache(seq_t *p) */

seq_t a(idx_t n)
{
    /* Cache of previously computed values */
    static seq_t *cache  = NULL;
    static idx_t  max_computed = (idx_t)-1;  /* largest index with value in cache */
    static size_t cap          = 0;          /* allocated capacity (elements) */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache      = alloc_cache(3);         /* room for indices 0,1,2 */
        cache[0]   = cache[1] = cache[2] = (seq_t)1;
        cap        = 3;
        max_computed = 2;                    /* a(0), a(1), a(2) known */
    }

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* grow by factors of 2 */

        seq_t *new_cache = alloc_cache(new_cap);
        /* copy existing values */
        for (idx_t i = 0; i <= max_computed; ++i) {
            new_cache[i] = cache[i];
        }
        /* base values are already copied; other slots are uninitialised */
        free_cache(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms in increasing order up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* i >= 3 here, because values below 3 were initialised */
            seq_t term =  (seq_t)2 * cache[i - 1]      /* 2*a(i-1) */
                        - cache[i - 3]                 /* minus a(i-3) */
                        + cache[i / 2];                /* plus a(floor(i/2)) */
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
