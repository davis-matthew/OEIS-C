/*
 * A308592  Number of total dominating sets in the (2n-1)-triangular snake
 *          (for n > 1).
 * https://oeis.org/A308592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* OEIS A308592:
   a(n) = 2*a(n-1) + 3*a(n-2) + 2*a(n-3)
   (a(1),a(2),a(3)) = (1,4,16)                               */

seq_t a(idx_t n)
{
    /* 1‑based memoisation table; cache[0] unused                     */
    static seq_t *cache      = NULL;
    static idx_t  max_comp   = 0;      /* highest index computed so far */
    static size_t cache_cap  = 0;      /* allocated size (including 0)  */

    /* Grow cache if the requested index is out of bounds               */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        /* Double size until we have room                                  */
        while ((size_t)n + 1 > cache_cap)
            cache_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) abort();         /* allocation failure */
        cache = new_cache;

        /* Initialise new slots (to zero – not strictly required)        */
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* Initialise base cases (only once)                                  */
    if (max_comp == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)16;
        max_comp = 3;
    }

    /* Compute all missing terms up to n (in case calls are out of order) */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] + 2 * cache[i - 3];
        }
        max_comp = n;
    }

    return cache[n];
}
