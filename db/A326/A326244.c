/*
 * A326244  Number of labeled n-vertex simple graphs without crossing or
 *          nesting edges.
 * https://oeis.org/A326244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 2 a(n) = 6*a(n-1) - 8*a(n-2) +
 *            4*a(n-3) for n > 2 (recurrence of A326244)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached sequence values */
    static idx_t  cache_size = 0; /* number of entries currently stored */

    if (n + 1 > cache_size) { /* need to extend cache up to index n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – caller should not need this path */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 1;
            } else if (i == 2) {
                cache[i] = 2;
            } else {
                /* recurrence: 6*a(i-1) - 8*a(i-2) + 4*a(i-3) */
                cache[i] =
                    6 * cache[i - 1] - 8 * cache[i - 2] + 4 * cache[i - 3];
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n]; /* return cached value */
}
