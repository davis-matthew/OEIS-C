/*
 * A351756  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 2*x)) / (1 - 2*x)^2.
 * https://oeis.org/A351756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k-1) * 2^(k-1) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 0..cache_last
                                 values of the sequence */
static size_t cache_cap = 0;  /* allocated capacity of cache    */
static idx_t  cache_last = 0; /* last index that has been computed */

/* ensure that the cache can store values up to idx inclusive */
static void extend_cache(idx_t idx)
{
    if ((size_t)idx + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while ((size_t)idx + 1 > new_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) is handled directly */
    if (n == 0)
        return (seq_t)1;

    if (!cache) {
        /* first allocation: store a(0) */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    if (n <= cache_last)
        return cache[n];

    extend_cache(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum  = 0;
        seq_t bin  = 1;   /* binomial(i,0) */
        seq_t pow2 = 1;   /* 2^0          */

        for (idx_t j = 0; j < i; ++j) {
            /* term = binomial(i,j) * 2^j * a(i-1-j) */
            seq_t term = bin;
            term *= pow2;
            term *= cache[i - 1 - j];
            sum += term;

            /* update binomial coefficient for next j */
            if (j + 1 < i) {
                /* bin = bin * (i - j) / (j + 1)  */
                bin = bin * (seq_t)(i - j) / (seq_t)(j + 1);
            }
            pow2 *= (seq_t)2;
        }
        cache[i] = sum;
    }
    cache_last = n;
    return cache[n];
}
