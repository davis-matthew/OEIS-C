/*
 * A400716  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - 2*x)) / (1 - x).
 * https://oeis.org/A400716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(0) = 1; for n >= 1 a(n) = a(n-1) + 2^(n-1) * Sum_{k=1}^{n-1}
 *            binomial(n-2, k-1) * a(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

/* Ensure that cache holds values up to index n and all needed entries are computed. */
static void ensure_cache(idx_t n)
{
    /* If the cache has not been created yet, allocate it. */
    if (cache == NULL) {
        /* Allocate a buffer that contains indices 0..n, rounded up to a power of two for
           amortised growth. */
        cache_cap = (size_t)(n < 2 ? 2 : n + 1);
        size_t cap = 1;
        while (cap < cache_cap) cap <<= 1;
        cache_cap = cap;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;           /* allocation failed – caller will handle */
        cache[0] = (seq_t)1;          /* a(0) */
        if (n >= 1) {
            cache[1] = (seq_t)1;      /* a(1) */
            last_computed = 1;
        } else {
            last_computed = 0;
        }
        return;
    }

    /* Grow the buffer if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failed – caller will handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Nothing to do if we have already computed up to n. */
    if (n <= last_computed) return;

    /* Compute new terms. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            seq_t bin = binomial(i - 2, k - 1);
            sum += bin * cache[k];
        }
        seq_t pow2_val = pow2(i - 1);   /* 2^(i-1) */
        cache[i] = cache[i - 1] + pow2_val * sum;
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
