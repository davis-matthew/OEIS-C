/*
 * A356820  Expansion of e.g.f. exp(-x * exp(3*x)).
 * https://oeis.org/A356820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (3*k)^(n-k) * binomial(n,k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Memoisation table for previously computed values. */
    static seq_t *cache  = NULL;         /* a(0) … a(cache_len‑1) */
    static size_t cache_cap = 0;          /* allocated capacity of cache */
    static idx_t  cache_len  = 0;         /* number of entries stored */

    /* Expand cache if necessary. */
    if (n + 1 > cache_cap) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Compute missing terms up to n (any call order). */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;

            for (idx_t k = 0; k <= i; ++k) {
                seq_t powv;

                /* Handle the special base 0 cases explicitly. */
                if (k == 0) {
                    if (i == 0)
                        powv = (seq_t)1;   /* 0^0 is defined as 1 here */
                    else
                        powv = (seq_t)0;   /* 0^e , e>0, is 0 */
                } else {
                    powv = big_pow((seq_t)(3 * k), (seq_t)(i - k));
                }

                seq_t bin = binomial(i, k);
                seq_t term = bin * powv;
                if (k & 1)          /* apply (-1)^k */
                    term = -term;
                sum += term;
            }

            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
