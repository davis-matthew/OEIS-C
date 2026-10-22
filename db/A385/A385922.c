/*
 * A385922  E.g.f. A(x) satisfies A(x) = exp(x*A(x) + x^5*A''''(x)).
 * https://oeis.org/A385922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k) * (1 - 6*k + 11*k^2 - 6*k^3 + k^4)
 *            * binomial(n-1,k) * a(k) * a(n-1-k), with a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Internal cache for computed terms. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t cache_computed_max = 0;

/* Ensure the cache can hold values up to index n. */
static void ensure_cache(idx_t n)
{
    if (seq_cache_cap <= (size_t)n) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();
        seq_cache = new_buf;
        seq_cache_cap = new_cap;
    }
}

/* Compute a(n) as defined by the recurrence, caching intermediate results. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    seq_t *cache = seq_cache;

    /* Initialize the base case if not already done. */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;

    /* If we already have computed up to n, just return it. */
    if (n <= cache_computed_max) return cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t m = cache_computed_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;  /* binomial(m-1, 0) */

        for (idx_t k = 0; k < m; ++k) {
            seq_t kk = (seq_t)k;
            /* (k^2 - 3k + 1)^2 part */
            seq_t t2 = kk * kk - (seq_t)3 * kk + (seq_t)1;
            seq_t w = (kk + (seq_t)1) * (t2 * t2);

            /* Compute contribution: w * bin * a(k) * a(m-1-k) */
            seq_t prod = w * bin;
            prod = prod * cache[k];
            prod = prod * cache[m - 1 - k];
            sum += prod;

            /* Update binomial coefficient for next k */
            if (k + 1 < m) {
                seq_t num = (seq_t)(m - 1 - k);
                seq_t den = (seq_t)(k + 1);
                bin = bin * num / den;  /* division is exact */
            }
        }
        cache[m] = sum;
    }

    cache_computed_max = n;
    return cache[n];
}
