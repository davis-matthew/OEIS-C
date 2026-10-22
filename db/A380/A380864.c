/*
 * A380864  a(n) = [x^n] sqrt(1 - 4*x)/(1 - 8*x). Row sums of A380865.
 * https://oeis.org/A380864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6 For n ≥ 2: a(n) = ((12 n − 6)·a(n‑1) − (32 n
 *            − 48)·a(n‑2)) / n This is the recurrence derived from (-48 + 32
 *            n)·a(n‑2) + (6‑12 n)·a(n‑1) + n·a(n) = 0. The function caches
 *            results so any order of calls works efficiently
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* external linkage expected */
seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored = cache_size */

    /* Need to extend the cache up to n? */
    if (cache == NULL || cache_size <= n)
    {
        size_t new_cap = (size_t)(n + 1 > 1 ? n + 1 : 2);

        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_size, new_cap);

        /* Initialise first two terms if cache just created */
        if (cache_size == 0)
        {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)6;
            cache_size = 2;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            /* Use 128‑bit intermediates to avoid overflow on 64‑bit builds */
            unsigned __int128 term1 = (unsigned __int128)(12 * i - 6)
                                    * (unsigned __int128)cache[i - 1];
            unsigned __int128 term2 = (unsigned __int128)(32 * i - 48)
                                    * (unsigned __int128)cache[i - 2];
            unsigned __int128 num   = term1 - term2;
            cache[i] = (seq_t)(num / i);
        }
        cache_size = n + 1;
    }
    return cache[n];
}
