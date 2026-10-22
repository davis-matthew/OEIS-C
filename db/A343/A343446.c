/*
 * A343446  Coefficients of the series S(p, q) for which -(p^(1/3))*S
 *          converges to the largest real root of x^4 - p*x + q, where 0 < p
 *          and 0 < q < 3*(p/4)^(4/3).
 * https://oeis.org/A343446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1, a(1) = 1, a(2) = 4
   a(n) = 4*(4*n−7)*(4*n−10)*(4*n−13)*a(n‑3),  n ≥ 3 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cached values */
static size_t  cache_len = 0;        /* number of values stored (length of cache) */

seq_t a(idx_t n)
{
    /* If we already have this value, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold up to index n. */
    size_t new_len = (size_t)n + 1;
    if (cache == NULL)
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;

    /* Compute missing values. */
    for (idx_t i = (idx_t)(cache_len > 0 ? cache_len - (new_len - (size_t)n) : 0);
         i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)-1;
        else if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)4;
        else
        {
            /* Use signed arithmetic for the three affine terms. */
            int64_t s1 = (int64_t)4 * (int64_t)i - 7;
            int64_t s2 = (int64_t)4 * (int64_t)i - 10;
            int64_t s3 = (int64_t)4 * (int64_t)i - 13;

            seq_t t1 = (seq_t)s1;
            seq_t t2 = (seq_t)s2;
            seq_t t3 = (seq_t)s3;

            seq_t prod = (seq_t)4;
            prod *= t1;
            prod *= t2;
            prod *= t3;

            cache[i] = prod * cache[i - 3];
        }
    }

    return cache[n];
}
