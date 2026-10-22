/*
 * A396569  Number of ways to tile a rectangular strip of length n, ending in
 *          a triangle, with three types of tiles.
 * https://oeis.org/A396569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 2*a(n-3) Initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
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

    /* Initialise cache on first call */
    if (cache == NULL) {
        idx_t init_size = (n + 1 > 3) ? (n + 1) : 3;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache_size = init_size;
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    if (n >= 3) {                                   /* values up to 2 are preset */
        for (idx_t i = 3; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)3
                     + cache[i-2] * (seq_t)4
                     + cache[i-3] * (seq_t)2;
        }
    }

    return cache[n];
}
