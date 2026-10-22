/*
 * A390837  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - x)^2) / (1 - x).
 * https://oeis.org/A390837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n+k-1,n-k-1) * 2^k * a(k); a(0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure the cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Grow capacity */
    size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
    while ((size_t)n >= new_cap)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;

    /* Compute new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else {
            seq_t sum   = 0;
            seq_t binom = 1;
            seq_t pow2  = 1;
            for (idx_t k = 0; k < i; ++k) {
                /* sum += binom * pow2 * a(k) */
                sum += binom * pow2 * cache[k];

                /* Prepare for next k if needed */
                if (k + 1 == i)
                    break;

                /* binom_{k+1} = binom_k * (i+k) * (i-k-1) /
                                 ((2*k+2)*(2*k+1)) */
                seq_t a_val = (seq_t)(i + k);
                seq_t b_val = (seq_t)(i - k - 1);
                seq_t c_val = (seq_t)(2 * k + 2);
                seq_t d_val = (seq_t)(2 * k + 1);

                binom = binom * a_val;
                binom = binom * b_val;
                binom = binom / (c_val * d_val);

                pow2 <<= 1;
            }
            cache[i] = sum;
        }
    }

    cache_size = n + 1;
}

/* Sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
