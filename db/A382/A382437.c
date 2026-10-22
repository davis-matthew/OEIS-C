/*
 * A382437  a(n) = a(n-1)^2 + 4 * a(n-1), with a(0) = 2.
 * https://oeis.org/A382437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 4*a(n-1), a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized terms */
    static size_t cache_size = 0; /* number of cached terms */

    /* Ensure we have terms up to index n */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Compute any missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)2;
            } else {
                seq_t prev = cache[i - 1];
                cache[i] = prev * prev + (seq_t)4 * prev;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
