/*
 * A362299  Number of tilings of a 3 X 2n rectangle using dominos and 2 X 2
 *          right triangles.
 * https://oeis.org/A362299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 15*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence value type, index type are provided by the build. */
extern seq_t a(idx_t n);   /* declaration for clarity */

/* Internal cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute a(n) using the recurrence and store in cache.  The cache
 * contains all values from 0 up to the largest n previously requested. */
seq_t a(idx_t n)
{
    /* Ensure cache is at least two entries for the base cases. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    } else if ((size_t)n + 1 > cache_cap) {
        /* Grow cache to accommodate n. */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        /* If realloc fails, we keep the old buffer; the sequence
         * calculation will still work, but memory usage may grow slowly. */
        if (tmp != NULL)
            cache = tmp;
        /* Initialize newly added entries. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = 10 * cache[i - 1] - 15 * cache[i - 2];
        }
        cache_cap = new_cap;
    }

    /* If cache is already large enough, we may still need to fill
     * intermediate values if this is the first request for large n. */
    for (size_t i = cache_cap; i <= (size_t)n; ++i) {
        if (i < 2) continue; /* base cases already set */
        cache[i] = 10 * cache[i - 1] - 15 * cache[i - 2];
    }

    return cache[(size_t)n];
}
