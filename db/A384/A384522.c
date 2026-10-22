/*
 * A384522  Expansion of e.g.f. 4/(7 - 3*exp(4*x)).
 * https://oeis.org/A384522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*∑_{k=1..n} 4^(k-1)*binomial(n,k)*a(n-k), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* next index to compute */

static void ensure_cache(size_t needed)
{
    if (cache_cap > needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise base case */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    if (n >= cache_len) {
        ensure_cache((size_t)n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;   /* C(i,0) */
            seq_t pow4  = 1;   /* 4^(k-1), starts with 4^0 */

            for (idx_t k = 1; k <= i; ++k) {
                /* binom = C(i,k) */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

                seq_t term = cache[i - k];
                term = term * binom;
                term = term * pow4;
                term = term * 3;

                sum += term;

                pow4 *= 4;
            }
            cache[i] = sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
