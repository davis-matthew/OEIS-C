/*
 * A356849  a(n) = a(n-1) - a(n-2) + 3*a(n-3) with a(0) = 1, a(1) = 2 and a(2)
 *          = 4.
 * https://oeis.org/A356849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 3*a(n-3), with a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If we already have the requested value, just return it. */
    if (n < cache_size)
        return cache[n];

    /* Need to grow the cache to hold up to n. */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;  /* Allocation failure – return 0 as a safe fallback. */

    /* Copy any previously computed terms. */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
    }

    /* Compute terms from the point we haven't computed yet. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_cache[0] = 1;
        } else if (i == 1) {
            new_cache[1] = 2;
        } else if (i == 2) {
            new_cache[2] = 4;
        } else {
            /* Recurrence: a(i) = a(i-1) - a(i-2) + 3*a(i-3) */
            new_cache[i] = new_cache[i-1] - new_cache[i-2] + 3 * new_cache[i-3];
        }
    }

    /* Release the old cache and store the new one. */
    free(cache);
    cache     = new_cache;
    cache_size = new_size;

    return cache[n];
}
