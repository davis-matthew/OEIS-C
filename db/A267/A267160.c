/*
 * A267160  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          107" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-4)-a(n-6) for n>=13 Initial values (indices
 *            0..12): 0, 2, 2, 2, 3, 3, 6, 2, 11, 4, 13, 2, 19
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;                /* number of stored values */

    /* Base values for n = 0..12 */
    const seq_t base_vals[13] = {
        0, 2, 2, 2, 3, 3, 6, 2, 11, 4, 13, 2, 19
    };

    /* Initialise cache with the base values on first call. */
    if (cache_len == 0) {
        cache_len = 13;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failure */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = base_vals[i];
    }

    /* For n < 13 we already have the value */
    if (n < 13)
        return cache[n];

    /* Extend cache to include index n */
    size_t idx = (size_t)n;
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                            /* allocation failure */
        cache = tmp;
        /* Compute new terms via the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t val = cache[i - 2] + cache[i - 4] - cache[i - 6];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[idx];
}
