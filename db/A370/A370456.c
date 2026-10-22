/*
 * A370456  a(0) = 1, a(n) = (1/2) * Sum_{j=1..n} (1-(-1)^j-(-2)^j) *
 *          binomial(n,j) * a(n-j) for n > 0.
 * https://oeis.org/A370456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, * a(n) = (1/2) * Σ_{j=1..n} (1-(-1)^j-(-2)^j) *
 *            binomial(n,j) * a(n-j), n>0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for memoisation */
static seq_t *cache    = NULL;
static size_t   cache_cap = 0;
static idx_t    cached_max = 0;

/* Ensure the cache can store indices up to n. */
static void ensure_cache_size(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        exit(EXIT_FAILURE); /* unable to allocate */

    /* if we already had some cache we copy it */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* initialize the newly allocated slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;

    /* make sure the base value a(0) == 1 */
    cache[0] = (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_cache_size(n);

    if (n <= cached_max)
        return cache[n];

    /* compute values from cached_max+1 up to n */
    for (idx_t m = cached_max + 1; m <= n; ++m) {
        seq_t sum_pos = 0;   /* contributions from odd j */
        seq_t sum_neg = 0;   /* contributions from even j */
        seq_t binom   = 1;   /* binomial(m,0) */
        seq_t pow2    = 1;   /* holds 2^j for current j */

        for (idx_t j = 1; j <= m; ++j) {
            /* update binomial to binomial(m,j) */
            binom = binom * (seq_t)(m - j + 1) / (seq_t)j;

            /* 2^j for this j */
            pow2 <<= 1;

            seq_t a_prev = (m - j == 0) ? (seq_t)1 : cache[m - j];
            seq_t term   = binom * a_prev;          /* binom * a(m-j) */

            if (j & 1U) { /* j odd: coefficient = 2 + 2^j */
                term = term * (pow2 + (seq_t)2);
                sum_pos += term;
            } else {      /* j even: coefficient = -2^j */
                term = term * pow2;
                sum_neg += term;
            }
        }

        seq_t total = sum_pos - sum_neg; /* guaranteed non‑negative */
        cache[m] = total / (seq_t)2;
    }

    cached_max = n;
    return cache[n];
}
