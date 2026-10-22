/*
 * A361532  Expansion of e.g.f. exp((x + x^2/2)/(1-x)).
 * https://oeis.org/A361532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1) * a(n-1) - (n-1)*(n-3) * a(n-2) - binomial(n-1,2)
 *            * a(n-3) for n > 2 Initial values: a(0) = 1, a(1) = 1, a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache for the sequence values */
static seq_t *cache = NULL;      /* array of seq_t values */
static idx_t cache_cap = 0;      /* number of elements allocated */

/* Ensure the cache is large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    /* Need space for indices 0..n inclusive */
    idx_t required = n + 1;
    if (required <= cache_cap)
        return;

    idx_t old_cap = cache_cap;
    cache_cap = required;

    if (cache == NULL) {
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) /* allocation failed: return zero as sentinel */
            return;
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) /* realloc failed: keep old cache */
            return;
        cache = tmp;
    }

    /* Initialise new entries to 0 */
    for (idx_t i = old_cap; i < cache_cap; ++i)
        cache[i] = 0;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    ensure_cache(n);
    if (!cache) /* allocation failed */
        return 0;

    /* Set base values if not already set */
    if (n >= 0 && cache[0] == 0)
        cache[0] = 1;
    if (n >= 1 && cache[1] == 0)
        cache[1] = 1;
    if (n >= 2 && cache[2] == 0)
        cache[2] = 4;

    /* Return base values directly */
    if (n == 0u) return cache[0];
    if (n == 1u) return cache[1];
    if (n == 2u) return cache[2];

    /* Compute missing values up to n */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0)
            continue;   /* already computed */

        seq_t term1 = (seq_t)(2 * (seq_t)i - 1) * cache[i - 1];
        seq_t term2 = (seq_t)(i - 1) * (i - 3) * cache[i - 2];
        seq_t bin   = ((seq_t)(i - 1) * (seq_t)(i - 2)) / 2;
        seq_t term3 = bin * cache[i - 3];

        cache[i] = term1 - term2 - term3;
    }

    return cache[n];
}
