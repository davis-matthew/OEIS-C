/*
 * A261054  Expansion of ( 2+x-x^2+x^3 ) / (1-x^2-x)^3 .
 * https://oeis.org/A261054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261054: Expansion of (2+x-x^2+x^3)/(1-x^2-x)^3
   Recurrence: a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)
   Initial values: a(0)=2, a(1)=7, a(2)=20, a(3)=51, a(4)=118, a(5)=260
   (derived from the generating function). */
#include <stdlib.h>

/* External linkage function as requested */
seq_t a(idx_t n)
{
    /* Small initial table */
    static const seq_t init[6] = {2, 7, 20, 51, 118, 260};

    /* Cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* number of stored values */

    /* Handle early indices directly */
    if (n < 6)
        return init[n];

    /* Allocate cache if first time */
    if (cache == NULL) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (cache == NULL)
            return 0;                    /* allocation failed – unlikely */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_size = 6;
    }

    /* Grow cache to include n */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                    /* allocation failed – unlikely */
        cache = tmp;

        /* Compute values from current cache_size up to n */
        for (size_t i = cache_size; i <= n; ++i) {
            cache[i] = (3 * cache[i - 1])
                       - (5 * cache[i - 3])
                       + (3 * cache[i - 5])
                       +  cache[i - 6];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
