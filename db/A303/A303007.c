/*
 * A303007  Expansion of (1-240*x)^(1/8).
 * https://oeis.org/A303007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -30*(8*n - 9)/n * a(n-1) (recurrence from OEIS) Initial
 *            value: a(0) = 1 The recurrence also satisfies n*a(n) +
 *            30*(-8*n+9)*a(n-1) = 0. The implementation caches computed terms
 *            to handle arbitrary call order
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 8, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache[0] = a(0) */
    static size_t cache_len = 0;      /* number of valid entries */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* initialise cache with a(0) if necessary */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if the requested value is already known, return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge the buffer if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;           /* double until big enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* factor = 30*(8*i - 9); compute using signed arithmetic */
        seq_t factor = (seq_t)30 * ( ((seq_t)i)*8 - (seq_t)9 );

        /* a(i) = factor / i * a(i-1)  (division is exact) */
        seq_t tmp = cache[i - 1] * factor;
        seq_t ai  = tmp / (seq_t)i;

        cache[i] = ai;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
