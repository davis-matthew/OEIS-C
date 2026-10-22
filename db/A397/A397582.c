/*
 * A397582  Number of ways to tile a 2 X n strip with squares, dominoes, and
 *          T-shaped tetrominoes.
 * https://oeis.org/A397582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) Initial values:
 *            a(0)=1, a(1)=2, a(2)=7, a(3)=24
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>

/* Internal cache for dynamic programming */
static seq_t *cache = NULL;    /* growing array of already computed a(n) */
static size_t  cache_len = 0;  /* number of valid entries in cache */
static size_t  cache_cap = 0;  /* allocated capacity of cache */

/* Ensure that the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Already computed? */
    if ((size_t)n < cache_len)
        return;

    /* How many entries are needed? */
    size_t need = (size_t)n + 1;   /* because n is inclusive */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new entries */
    for (size_t i = cache_len; i < need; ++i) {
        if (i == 0)          cache[i] = 1;
        else if (i == 1)      cache[i] = 2;
        else if (i == 2)      cache[i] = 7;
        else if (i == 3)      cache[i] = 24;
        else {
            seq_t term = (seq_t)3 * cache[i - 1] + (seq_t)2 * cache[i - 2];
            cache[i] = term - cache[i - 3] - cache[i - 4];
        }
    }
    cache_len = need;
}

/* Internal implementation of the recurrence; uses the cache. */
static seq_t a_impl(idx_t n)
{
    if (n == 0)  return 1;   /* early return for no allocation */
    ensure_cache(n);
    return cache[(size_t)n];
}

/* Public wrapper that matches the linkage used by the prelude
   (C++ linkage if compiled as C++, C linkage otherwise). */
#ifdef __cplusplus
seq_t a(idx_t n) { return a_impl(n); }
#else
seq_t a(idx_t n) { return a_impl(n); }
#endif
