/*
 * A373245  Binomial transform of A135318.
 * https://oeis.org/A373245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 a(n-1) - 5 a(n-2) + 2 a(n-3) + 2 a(n-4), for n ≥ 4
 *            Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=9 This recurrence
 *            follows from the generating function of A373245
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms already computed */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* first time: allocate and initialize the first four terms */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache_len = 4;
    }

    /* if requested index is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* ensure we have enough memory */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* a(i) = 4*a(i-1) - 5*a(i-2) + 2*a(i-3) + 2*a(i-4) */
        seq_t t = (seq_t)4 * cache[i - 1];
        t += (seq_t)2 * cache[i - 3];
        t += (seq_t)2 * cache[i - 4];
        t -= (seq_t)5 * cache[i - 2];
        cache[i] = t;
    }

    cache_len = n + 1;
    return cache[n];
}
