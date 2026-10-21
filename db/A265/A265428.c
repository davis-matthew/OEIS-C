/*
 * A265428  Number of ON (black) cells in the n-th iteration of the "Rule 188"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) (n > 4) Initial values: a(0)=1,
 *            a(1)=2, a(2)=2, a(3)=4, a(4)=4
 * Range:     u8 n <= 338, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..4 */
    static const seq_t init[5] = {1, 2, 2, 4, 4};

    if (n < 5) {
        return init[n];
    }

    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with first five values on first call */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* Out‑of‑memory: abort (unlikely in test harness) */
            return 0;
        }
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
    }

    /* Grow the cache if a higher index is requested */
    if (n >= cache_size) {
        seq_t *new_cache =
            (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Out‑of‑memory: abort */
            return 0;
        }
        cache = new_cache;

        /* Compute missing values using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
