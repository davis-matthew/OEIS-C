/*
 * A297554  a(n) = a(n-2) + 4*a(n-3) - 4*a(n-5), where a(0) = 1, a(1) = 4,
 *          a(2) = 7, a(3) = 12, a(4) = 19, a(5) = 28.
 * https://oeis.org/A297554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 4*a(n-3) - 4*a(n-5) a(0) = 1, a(1) = 4, a(2) =
 *            7, a(3) = 12, a(4) = 19, a(5) = 28
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 45, u64 n <= 93, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute a(n) for any n≥0.  The function keeps a static cache so that
   calls in arbitrary order are handled efficiently. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static size_t cache_cap = 0;     /* allocated capacity (in elements)   */
    static size_t cache_len = 0;     /* number of values currently stored  */

    /* allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* grow cache capacity to hold index n (zero‑based) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the first six terms once */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 7;
        cache[3] = 12;
        cache[4] = 19;
        cache[5] = 28;
        cache_len = 6;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 2];
        seq_t t2 = cache[i - 3];
        seq_t t3 = cache[i - 5];
        cache[i] = t1 + (seq_t)4 * t2 - (seq_t)4 * t3;
    }

    /* Update stored length after computation */
    if (cache_len <= (size_t)n) cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
