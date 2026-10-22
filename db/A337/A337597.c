/*
 * A337597  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k *
 *          6^(k-1) * a(n-k).
 * https://oeis.org/A337597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/n) * Σ_{k=1..n} binom(n,k)^2 * k * 6^(k-1) *
 *            a(n-k) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* array of computed a(n) values */
static idx_t cache_len = 0;        /* current allocated size of cache (≥ 1 after init) */
static idx_t computed = 0;         /* highest index for which a(i) is filled */

/* Compute a(n) with memoization.  The function will keep a static cache
   and extend it only as far as needed. */
seq_t a(idx_t n)
{
    /* Initial allocation for a(0) if needed */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* Grow cache to hold a(n) */
    if (n >= cache_len) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = newlen;
        /* New positions will be filled in the loop below */
    }

    /* Compute missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;    /* binom(m,0) */
        seq_t pow6  = 1;    /* 6^(k-1) when k=1 */

        for (idx_t k = 1; k <= m; ++k) {
            /* binom = binom(m, k) */
            binom = binom * (m + 1 - k) / k;   /* exact division */

            /* term = binom^2 * k * 6^(k-1) * a(m-k) */
            seq_t term = binom * binom;
            term = term * (seq_t)k;
            term = term * pow6;
            term = term * cache[m - k];

            sum += term;
            pow6 = pow6 * 6;   /* advance 6^(k-1) -> 6^k for next iteration */
        }

        cache[m] = sum / (seq_t)m;   /* exact division by m */
    }

    computed = n;
    return cache[n];
}
