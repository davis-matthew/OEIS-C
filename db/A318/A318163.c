/*
 * A318163  a(0) = a(3) = 0, a(1) = a(2) = 1; for n >= 2, a(2*n) = -a(n-1) and
 *          a(2*n+1) = -a(n-1)-a(n).
 * https://oeis.org/A318163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=0; for n≥2, a(2*n) = -a(n-1)
 *            a(2*n+1) = -a(n-1)-a(n)
 * Range:     i8 n <= 3368, i16 n <= 9172, i32 n <= 9172, i64 n <= 9172,
 *            i128 n <= 9172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises results, so any order of calls is fast. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;      /* cache size (number of stored elements) */

    /* Grow the cache if we need a value beyond the current limit */
    if ((idx_t)cap <= n)
    {
        size_t new_cap = (size_t)n + 1;          /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)                /* allocation failed */
            abort();

        cache = tmp;

        /* Compute missing values from old cap up to new_cap-1 */
        for (size_t i = cap; i < new_cap; ++i)
        {
            if (i == 0)
                cache[i] = 0;
            else if (i == 1 || i == 2)
                cache[i] = 1;
            else if (i == 3)
                cache[i] = 0;
            else if ((i & 1) == 0)             /* i is even: 2*m */
            {
                size_t m = i >> 1;            /* m = i/2  (i>=4 => m>=2) */
                cache[i] = -cache[m - 1];
            }
            else                                 /* i is odd: 2*m+1 */
            {
                size_t m = i >> 1;            /* m = floor(i/2) (i>=5 => m>=2) */
                cache[i] = -cache[m - 1] - cache[m];
            }
        }

        cap = new_cap;
    }

    return cache[(size_t)n];
}
