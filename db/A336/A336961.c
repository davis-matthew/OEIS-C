/*
 * A336961  Expansion of e.g.f. exp(x * (2 + x) * exp(x)).
 * https://oeis.org/A336961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1) * k * (k+1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static cache for computed values */
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t cache_len = 0;      /* number of valid entries in cache */

/* Resolve sequence term a(n) */
seq_t a(idx_t n)
{
    /* ensure caches at least one element (a(0) = 1) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* enlarge cache and compute missing terms up to n */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* fallback, should not happen in tests */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;  /* binomial(i-1, k-1) */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = binom;
                term = term * (seq_t)k;
                term = term * (seq_t)(k + 1);
                term = term * cache[(size_t)(i - k)];
                sum += term;

                /* update binomial for next k, if any */
                if (k < i) {
                    binom = binom * (seq_t)(i - k);
                    binom = binom / (seq_t)k;
                }
            }
            cache[(size_t)i] = sum;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
