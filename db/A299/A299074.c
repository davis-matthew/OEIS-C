/*
 * A299074  Expansion of 1/((1-x)*(1-2*x)*(1-6*x)*(1-24*x)).
 * https://oeis.org/A299074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n-1) - 236*a(n-2) + 492*a(n-3) - 288*a(n-4) (n>3) *
 *            Initial values: a(0)=1, a(1)=33, a(2)=853, a(3)=20853
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_cache, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/alloc_cache.h" /* seq_t *alloc_cache(idx_t n) */

/* Compute the first four terms by the explicit formula
 * a(n) = (115*2^n - 759*6^n + 1920*24^n - 11) / 1265
 */
static seq_t base_term(idx_t n)
{
    seq_t pow2  = seq_pow((seq_t)2, n);
    seq_t pow6  = seq_pow((seq_t)6, n);
    seq_t pow24 = seq_pow((seq_t)24, n);

    /* numerator = 115*2^n + 1920*24^n - 759*6^n */
    seq_t num = (seq_t)115 * pow2;
    num += (seq_t)1920 * pow24;
    num -= (seq_t)759 * pow6;

    /* subtract the constant term 11, then divide */
    num -= (seq_t)11;
    return num / (seq_t)1265;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with the first four terms */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)alloc_cache(cache_len);
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = base_term((idx_t)i);
    }

    /* return already computed value */
    if ((size_t)n < cache_len)
        return cache[n];

    /* extend cache to include term n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute terms from the current cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] =   (seq_t)33  * cache[i-1]
                    - (seq_t)236 * cache[i-2]
                    + (seq_t)492 * cache[i-3]
                    - (seq_t)288 * cache[i-4];
    }
    cache_len = new_len;

    return cache[n];
}
