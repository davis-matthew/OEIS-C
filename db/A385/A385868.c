/*
 * A385868  Number of ways to tile a hexagonal strip made up of n equilateral
 *          triangles, using triangles, diamonds, and trapezoids.
 * https://oeis.org/A385868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-4) + 9*a(n-8) + 32*a(n-12) + 9*a(n-16) + a(n-20) -
 *            a(n-24) (a(0)=1, a(1)=1, a(2)=2, a(3)=4, …, a(23)=3996430)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t const init[24] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
    (seq_t)7, (seq_t)13, (seq_t)39, (seq_t)66,
    (seq_t)110, (seq_t)200, (seq_t)604, (seq_t)1032,
    (seq_t)1741, (seq_t)3149, (seq_t)9476, (seq_t)16202,
    (seq_t)27337, (seq_t)49461, (seq_t)148841, (seq_t)254466,
    (seq_t)429308, (seq_t)776774, (seq_t)2337580, (seq_t)3996430
};

/* cache keeps already computed values so that repeated calls are fast.
   The cache grows only when a larger index is requested.              */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* enlarge cache to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    /* initialise added entries to 0 (will be overwritten when computed) */
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initial cache allocation */
    if (!cache) {
        cache_cap = 24;               /* room for known terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 24; ++i) cache[i] = init[i];
    }

    /* if a(n) already known, return it */
    if ((size_t)n < cache_cap && cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    /* make sure cache can store n */
    ensure_cache(n);

    /* compute any missing values up to n */
    for (size_t i = 24; i <= (size_t)n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed */
        seq_t v = (15 * cache[i - 4] +
                   9  * cache[i - 8] +
                  32 * cache[i -12] +
                   9 * cache[i -16] +
                   cache[i -20]) - cache[i -24];
        cache[i] = v;
    }
    return cache[n];
}
