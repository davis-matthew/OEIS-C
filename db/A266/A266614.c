/*
 * A266614  Number of ON (black) cells in the n-th iteration of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-4)-a(n-6) for n>5, with a(0)=1, a(1)=0,
 *            a(2)=2, a(3)=3, a(4)=1, a(5)=8
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initial values */
    static const seq_t init_vals[6] = {1, 0, 2, 3, 1, 8};

    /* quick return for the first six terms */
    if (n < 6)
        return init_vals[n];

    /* extend the cache if necessary */
    if (cache_cap <= n) {
        idx_t old_cap = cache_cap;
        cache_cap = (n + 1 > 6) ? (n + 1) : 6;   /* at least 6 entries */

        /* allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) /* allocation failed – aborting is OK for simplicity */
            abort();
        cache = tmp;

        /* initialise the first 6 entries if they were not already set */
        if (old_cap < 6) {
            for (idx_t i = old_cap; i < 6; ++i)
                cache[i] = init_vals[i];
        }

        /* compute new entries up to cache_cap-1 */
        for (idx_t i = old_cap; i < cache_cap; ++i) {
            if (i < 6)   /* already initialised */
                continue;
            cache[i] = cache[i - 2] + cache[i - 4] - cache[i - 6];
        }
    }

    return cache[n];
}
