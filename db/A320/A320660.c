/*
 * A320660  Number of business cards required to build an origami level n
 *          Jerusalem cube.
 * https://oeis.org/A320660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 16*a(n-2) - 80*a(n-3) - 48*a(n-4); a(0) = 12,
 *            a(1) = 72, a(2) = 672, a(3) = 6048, a(4) = 55488
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation of computed terms   */
    static seq_t *cache          = NULL;   /* array a(0)..a(computed_until) */
    static size_t  cache_cap     = 0;       /* allocated size of cache   */
    static idx_t   computed_until = (idx_t)-1; /* highest index computed */

    /* initialise cache lazily */
    if (cache == NULL) {
        /* create room for the first five terms */
        cache_cap = 5;
        cache     = alloc_seq(cache_cap);
        if (!cache) abort();                 /* out of memory */

        cache[0] = (seq_t)12;
        cache[1] = (seq_t)72;
        cache[2] = (seq_t)672;
        cache[3] = (seq_t)6048;
        cache[4] = (seq_t)55488;
        computed_until = 4;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        if (!new_cache) abort();

        for (size_t i = 0; i <= computed_until; ++i)
            new_cache[i] = cache[i];

        /* free old cache */
        free_seq(cache);
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* compute any missing values up to n using the linear recurrence */
    if (computed_until < n) {
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            cache[i] = 12 * cache[i-1] - 16 * cache[i-2]
                     - 80 * cache[i-3] - 48 * cache[i-4];
        }
        computed_until = n;
    }

    return cache[n];
}
