/*
 * A378683  a(0) = 1, a(n+1) = 6*a(n)^3 - 3*a(n).
 * https://oeis.org/A378683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = 6*a(n)^3 - 3*a(n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization cache */
static size_t cache_size = 0; /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* ensure cache is initialized */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* out of memory */
        cache[0] = 1; /* a(0) = 1 */
    }

    /* extend cache up to index n if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t sq   = prev * prev;          /* a(n)^2 */
            seq_t cu   = sq * prev;            /* a(n)^3 */
            seq_t term = 6 * cu - 3 * prev;    /* a(n+1) */
            cache[i] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
