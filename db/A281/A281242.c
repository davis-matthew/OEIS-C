/*
 * A281242  Solutions y to the negative Pell equation y^2 = 72*x^2 - 1331712
 *          with x,y >= 0.
 * https://oeis.org/A281242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6. Initial values: a(1)=0,
 *            a(2)=576, a(3)=1920, a(4)=3264, a(5)=5376, a(6)=12096
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 27, u64 n <= 65, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 1‑based cache of computed values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Initialise the cache with the first six terms. */
static void init_cache(void)
{
    cache_len = 6;
    cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
    if (!cache) abort();
    cache[0] = 0;                 /* unused */
    cache[1] = 0;
    cache[2] = 576;
    cache[3] = 1920;
    cache[4] = 3264;
    cache[5] = 5376;
    cache[6] = 12096;
}

/* Grow the cache to store terms up to (and including) `needed`. */
static void ensure_cache(idx_t needed)
{
    if (cache_len >= needed)
        return;
    if (!cache)
        init_cache();                 /* after this, cache_len == 6 */
    if (cache_len >= needed)          /* may happen if needed <= 6 */
        return;

    size_t new_len = (size_t)needed + 1;         /* include index 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= needed; ++i)
    {
        /* a(i) = 6*a(i-3) - a(i-6) */
        cache[i] = 6 * cache[i - 3] - cache[i - 6];
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* not part of the sequence, but harmless */
    ensure_cache(n);
    return cache[n];
}
