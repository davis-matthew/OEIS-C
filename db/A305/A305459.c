/*
 * A305459  a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) + a(n-2).
 * https://oeis.org/A305459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3 * n * a(n-1) + a(n-2)
   with base values a(0) = 1, a(1) = 3.  The function computes terms
   on demand and memoizes them in a static cache.  It is safe for
   any unsigned idx_t (uint64_t) and any seq_t that supports the
   required arithmetic operators.  */

#include <stdlib.h>

/* static cache state */
static seq_t *cache = NULL;   /* array of computed terms   */
static size_t cache_cap = 0;  /* current capacity of array */
static size_t cache_max = 0;  /* largest index that has already been computed */

/* helper that grows the cache to accommodate at least new_cap elements */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;

    /* grow by doubling for amortised O(1) */
    size_t cap = cache_cap ? cache_cap : 4;
    while (cap < new_cap)
        cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed – terminate */
        exit(1);

    cache = new_cache;
    cache_cap = cap;
}

/* compute the sequence term a(n) */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* initialise cache on first use */
    if (!cache)
    {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 4)
            init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_cap = init_cap;
        cache_max = 1;
    }

    /* ensure the array can hold index n */
    ensure_cache_cap((size_t)n + 1);

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_max + 1); i <= n; ++i)
    {
        seq_t term = (seq_t)3 * (seq_t)i * cache[i - 1] + cache[i - 2];
        cache[i] = term;
        cache_max = (size_t)i;
    }

    return cache[n];
}
