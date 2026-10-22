/*
 * A376111  a(0) = 1; a(n) = Sum_{k=1..n} (2^k-1) * a(k-1) * a(n-k).
 * https://oeis.org/A376111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (2^k-1)·a(k-1)·a(n-k) a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated slots */
static size_t cache_valid = 0;        /* number of computed terms */

/* Pre‑computed powers 2^k – 1, indexed by k */
static seq_t *pow2minus1 = NULL;
static size_t pow_cap = 0;            /* allocated slots in the power table */

/* Ensure the cache has room for indices 0 … n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_cap = n + 1;
        cache_valid = 1;              /* only a(0) computed yet */
        return;
    }
    if (cache_cap < (size_t)(n + 1)) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }
}

/* Ensure the power table has entries 0 … k (inclusive) */
static void ensure_pow2minus1(idx_t k)
{
    if (k + 1 <= pow_cap)
        return;

    size_t new_cap = (size_t)(k + 1);
    pow2minus1 = (seq_t*)realloc(pow2minus1, new_cap * sizeof(seq_t));

    if (pow_cap == 0) {
        /* 2^0 – 1 = 0, 2^1 – 1 = 1 */
        pow2minus1[0] = (seq_t)0;
        if (new_cap > 1)
            pow2minus1[1] = (seq_t)1;
        pow_cap = (new_cap > 1) ? 2 : 1;
    }

    /* 2^i – 1 = 2·(2^(i-1) – 1) + 1 for i ≥ 2 */
    for (idx_t i = pow_cap; i <= k; ++i) {
        pow2minus1[i] = pow2minus1[i - 1] * (seq_t)2 + (seq_t)1;
    }
    pow_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Ensure the cache is initialised for consistency. */
        if (!cache) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            cache[0] = (seq_t)1;
            cache_cap = 1;
            cache_valid = 1;
        }
        return cache[0];
    }

    /* Prepare the cache and power table. */
    ensure_cache(n);

    /* Compute any terms not yet computed. */
    for (size_t m = cache_valid; m <= (size_t)n; ++m) {
        ensure_pow2minus1((idx_t)m);      /* need powers up to m */
        seq_t sum = 0;
        for (idx_t k = 1; k <= (idx_t)m; ++k) {
            seq_t term = pow2minus1[(size_t)k];
            seq_t prod = term * cache[(size_t)(k - 1)];
            prod = prod * cache[(size_t)(m - k)];
            sum = sum + prod;
        }
        cache[(size_t)m] = sum;
    }
    cache_valid = (size_t)n + 1;
    return cache[(size_t)n];
}
