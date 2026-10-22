/*
 * A379741  Number of cyclic edge cuts in the (2n-1)-triangular snake graph.
 * https://oeis.org/A379741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 271*a(n-2) + 1197*a(n-3) - 1960*a(n-4)
 *            Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t value by a small positive integer m (m <= 1960) */
static seq_t mul_small(seq_t x, int m)
{
    seq_t res = 0;
    for (int i = 0; i < m; ++i)
        res += x;
    return res;
}

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of valid entries (0 .. cache_len-1) */

/* main sequence function */
seq_t a(idx_t n)
{
    /* 0 for the initial three terms */
    if (n <= 3)
        return 0;

    /* Ensure cache contains indices up to n */
    if (!cache || cache_len <= n)
    {
        size_t new_len = (size_t)(n + 1);   /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;

        /* if this is the first allocation, initialize base cases */
        if (cache_len == 0)
        {
            cache[0] = 0;      /* unused */
            cache[1] = 0;
            cache[2] = 0;
            cache[3] = 0;
            cache[4] = 3;
            cache_len = 5;     /* indices 0..4 are valid */
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i <= 4)
            {
                cache[i] = (i == 4) ? (seq_t)3 : (seq_t)0;
            }
            else
            {
                seq_t t1 = mul_small(cache[i - 1], 27);
                seq_t t2 = mul_small(cache[i - 2], 271);
                seq_t t3 = mul_small(cache[i - 3], 1197);
                seq_t t4 = mul_small(cache[i - 4], 1960);
                cache[i] = t1 - t2 + t3 - t4;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
