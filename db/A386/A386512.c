/*
 * A386512  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * binomial(k+3,4)
 *          * binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0..n-1} (1 + k) * binomial(k+3,4)
 *            * binomial(n-1,k) * a(k) * a(n-1-k);
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* core helpers are provided by the build system:
   seq_t binom(idx_t a, idx_t b); */
#include <stdlib.h>
#include "../utils/binom.h"

/* Memoization buffers */
static seq_t *cache      = NULL;  /* a[0..max]      */
static size_t cache_cap = 0;      /* number of slots for cache   */
static idx_t  cached    = 0;      /* largest index computed so far*/
static seq_t *weight     = NULL;  /* weight[k] = (1+k)*binom(k+3,4) */
static size_t weight_cap = 0;      /* number of slots for weight  */

/* Grow the cache to hold indices 0..n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Grow the weight array to hold indices 0..n */
static void ensure_weight_size(idx_t n)
{
    if ((size_t)n + 1 > weight_cap) {
        size_t old_cap = weight_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *new_w = (seq_t *)realloc(weight, new_cap * sizeof(seq_t));
        if (!new_w) exit(EXIT_FAILURE);
        weight = new_w;
        weight_cap = new_cap;
        for (size_t k = old_cap; k <= (size_t)n; ++k) {
            weight[k] = (seq_t)(1 + k) * binom((idx_t)k + 3, 4);
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* First call: initialise with a(0)=1 */
    if (!cache) {
        ensure_cache_size(0);
        cache[0] = (seq_t)1;
        cached   = 0;
        ensure_weight_size(0);  /* weight[0] will be built (zero)       */
    }

    if (n == 0) return cache[0];

    /* Make sure structures are large enough */
    ensure_cache_size(n);
    ensure_weight_size(n);

    /* Compute all values up to n if not already done */
    for (idx_t m = cached + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m - 1; ++k) {
            seq_t term = weight[k] *
                         binom(m - 1, k) *
                         cache[k] *
                         cache[m - 1 - k];
            sum += term;
        }
        cache[m] = cache[m - 1] + sum;
    }
    cached = n;
    return cache[n];
}
