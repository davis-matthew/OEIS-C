/*
 * A352658  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 *
 *          binomial(k+1,2) * k * a(n-k).
 * https://oeis.org/A352658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1..n} binomial(n,k)^2 * binomial(k+1,2) * k * a(n-k)
 */

#include <stddef.h>
#include <stdlib.h>

/* Returns a(n) with memoisation.  Works with arbitrary precision seq_t. */
seq_t a(idx_t n)
{
    /* Cached values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current allocated size */

    /* Ensure the cache is large enough and compute missing terms. */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;
            seq_t bin = 1;  /* binomial(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* bin = binomial(i,k) from binomial(i,k-1) */
                bin = bin * (seq_t)(i - k + 1);
                bin = bin / (seq_t)k;

                seq_t term = bin * bin;              /* binomial(i,k)^2 */
                seq_t b = (seq_t)(k + 1) * (seq_t)k / 2UL;   /* binomial(k+1,2) */
                term   = term * b * (seq_t)k;        /* *k * binomial(k+1,2) */
                term   = term * cache[i - k];        /* *a(i-k) */
                sum   += term;
            }
            cache[i] = sum / (seq_t)i;  /* divide by n = i */
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
