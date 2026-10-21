/*
 * A247525  a(n) = 5 * a(n-1) - 2 * a(n-1)^2 / a(n-2), with a(0) = 1, a(1) =
 *          2.
 * https://oeis.org/A247525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 5 * a(n-1) - 2 * a(n-1)^2 / a(n-2) Initial terms: a(0) =
 *            1, a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 9, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_used = 0;   /* number of cached terms */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_cap = 4;                     /* start with space for 4 terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_used = 2;
    }

    /* grow capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t newcap = cache_cap * 2;
        while (newcap <= n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms up to n */
    if (n + 1 > cache_used) {
        for (idx_t i = cache_used; i <= n; ++i) {
            /* i is already >= 2 because 0 and 1 were cached */
            seq_t prev     = cache[i - 1];
            seq_t prevprev = cache[i - 2];
            /* Perform division first to avoid large intermediate values */
            seq_t q = prev / prevprev;           /* integer division */
            seq_t prod = q * prev * 2;           /* 2 * q * prev */
            cache[i] = 5 * prev - prod;
        }
        cache_used = n + 1;
    }

    return cache[n];
}
