/*
 * A340309  Number of ordered pairs of vertices which have two different
 *          shortest paths between them in the n-Hanoi graph (3 pegs, n
 *          discs).
 * https://oeis.org/A340309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 17*a(n-2) + 6*a(n-3), a(1)=0, a(2)=6, a(3)=48
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */
extern seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently cached */

    /* Handle requests below the OEIS offset */
    if (n == 0) return 0;

    /* Initialise cache with the first three terms if not yet done */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();                      /* simple failure handling */
        cache[0] = (seq_t)0;                      /* a(1) */
        cache[1] = (seq_t)6;                      /* a(2) */
        cache[2] = (seq_t)48;                     /* a(3) */
    }

    /* Return already cached value if requested */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow cache up to the requested index */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* Compute a(i+1) = 8*a(i) - 17*a(i-1) + 6*a(i-2) */
        seq_t term = cache[i - 1] * (seq_t)8 + cache[i - 3] * (seq_t)6;
        term -= cache[i - 2] * (seq_t)17;
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n - 1];
}
