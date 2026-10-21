/*
 * A255163  Irrational parts of circle radii in nested circles and hexagons
 *          (see comment).
 * https://oeis.org/A255163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -12*a(n-1) + 12*a(n-2), a(0)=1, a(1)=2
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS A255163 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity (in terms) */
    static size_t cache_len = 0;  /* number of terms actually stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = (size_t)2;                /* enough for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();           /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    while (cache_len <= (size_t)n) {
        seq_t prev     = cache[cache_len - 1];
        seq_t prevprev = cache[cache_len - 2];
        cache[cache_len] = (seq_t)(-12) * prev + (seq_t)12 * prevprev;
        ++cache_len;
    }

    return cache[(size_t)n];
}
