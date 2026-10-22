/*
 * A309712  Number of odd parts appearing among the third largest parts of the
 *          partitions of n into 4 parts.
 * https://oeis.org/A309712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + 2*a(n-8) -
 *            2*a(n-9) + a(n-10) - a(n-14) + 2*a(n-15) - a(n-16), for n > 15.
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1,
 *            a(6)=2, a(7)=2, a(8)=3, a(9)=3, a(10)=5, a(11)=6, a(12)=9,
 *            a(13)=10, a(14)=13, a(15)=14, a(16)=18. This functio...
 * Range:     u8 n <= 40, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* pointer to memoised values */
static size_t  cache_cap = 0;    /* capacity of allocated array */
static size_t  cache_len = 0;    /* number of values currently computed */

/* initial values for n = 0 … 16 */
static const seq_t init[17] = {
    (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)1,
    (seq_t)1,  (seq_t)2,  (seq_t)2,  (seq_t)3,  (seq_t)3,
    (seq_t)5,  (seq_t)6,  (seq_t)9,  (seq_t)10, (seq_t)13,
    (seq_t)14, (seq_t)18
};

/* grow the cache up to at least n+1 entries and compute missing values */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise the cache with the first 17 values */
        size_t cap = (n < 16 ? 17 : n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i <= 16; ++i) cache[i] = init[i];
        for (idx_t i = 17; i <= n; ++i) cache[i] = 0;
        cache_cap = cap;
        cache_len = 17;
    } else if ((idx_t)cache_cap <= n) {
        /* increase capacity */
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_cap; i <= n; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute values from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 16) continue;           /* already initialised */

        /* sum of terms with positive coefficients */
        seq_t pos = cache[i - 1];
        pos += cache[i - 1];
        pos += cache[i - 6];
        pos += cache[i - 8];
        pos += cache[i - 8];
        pos += cache[i - 10];
        pos += cache[i - 15];
        pos += cache[i - 15];

        /* sum of terms with negative coefficients */
        seq_t neg = cache[i - 2];
        neg += cache[i - 7];
        neg += cache[i - 7];
        neg += cache[i - 9];
        neg += cache[i - 9];
        neg += cache[i - 14];
        neg += cache[i - 16];

        cache[i] = pos - neg;            /* pos >= neg for all n */
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
