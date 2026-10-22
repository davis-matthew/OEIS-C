/*
 * A394531  Sum_{k=1..n} k^2 * a(k)/n^(2*k) = 1.
 * https://oeis.org/A394531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*(n-1)) - Sum_{k=1..n-1} n^(2*(n-1-k)) * k^2 * a(k),
 *            with a(1) = 1. The function memoizes results in a static array
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache; cache[0] unused */
static size_t cache_cap = 0;    /* allocated size (number of slots) */

/* Ensure that cache has room for indices up to n (inclusive). */
static void cache_ensure(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = (size_t)n + 1;   /* slots 0..n, where 0 is unused */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                     /* allocation failure */

    /* initialise new entries to 0 ("uncomputed") */
    for (size_t i = 0; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    /* copy existing computed values */
    if (cache) {
        for (size_t i = 1; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 */

    cache_ensure(n);

    /* initialise a(1) if not already done */
    if (cache[1] == (seq_t)0)
        cache[1] = (seq_t)1;

    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute all values up to n; previous values < n are already computed */
    for (idx_t m = 2; m <= n; ++m) {
        if (cache[m] != (seq_t)0)
            continue;   /* already known */

        seq_t base = (seq_t)m * (seq_t)m;   /* m^2 */

        seq_t pow_ = (seq_t)1;   /* base^(m-1-k) ; start with k=m-1 -> exponent 0 */
        seq_t sum = (seq_t)0;

        for (idx_t k = m - 1; k > 0; --k) {
            seq_t term = cache[k];
            term *= (seq_t)k;
            term *= (seq_t)k;
            term *= pow_;
            sum += term;
            pow_ *= base;          /* advance to next power for the next k   */
        }

        /* after loop, pow_ == m^(2*(m-1)) */
        cache[m] = pow_ - sum;
    }

    return cache[n];
}
