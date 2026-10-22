/*
 * A351937  Expansion of e.g.f. exp( (sinh(x) + x*cosh(x)) / 2 ).
 * https://oeis.org/A351937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊(n−1)/2⌋} binomial(n−1,2k) * (k+1) * a(n−2k−1),
 *            a(0) = 1 Computation uses memoised recurrence with incremental
 *            binomial calculation
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values a[0..cache_len-1] */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static size_t cache_cap = 0;

    /* Return already known value */
    if (n < cache_len)
        return cache[n];

    /* Ensure enough capacity in the cache */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {               /* a(0) = 1 */
            cache[0] = 1;
            continue;
        }

        idx_t max_k = (i - 1) / 2;
        seq_t sum = 0;
        seq_t binom = 1;            /* C(i‑1,0) */

        for (idx_t k = 0; k <= max_k; ++k) {
            /* term = binom * (k+1) * a(i‑2k‑1) */
            seq_t term = binom;
            term *= (seq_t)(k + 1);
            idx_t ref = i - (2 * k + 1);   /* i‑2k‑1 */
            term *= cache[ref];
            sum += term;

            /* Prepare next binomial C(i‑1,2(k+1)) */
            if (k == max_k) break;

            seq_t num1   = (seq_t)(i - 1 - 2 * k);
            seq_t num2   = (seq_t)(i - 2 - 2 * k);
            seq_t denom1 = (seq_t)(2 * k + 1);
            seq_t denom2 = (seq_t)(2 * k + 2);

            seq_t numer = binom * num1;
            numer *= num2;
            binom = numer / (denom1 * denom2);
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
