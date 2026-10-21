/*
 * A264596  Let S_n be the list of the first n nonnegative numbers written in
 *          binary, with least significant bits on the left, and sorted into
 *          lexicographic order; a(n) = position of n in S_n, starting
 *          indexing at 0.
 * https://oeis.org/A264596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(2n) = a(n); a(2n+1) = a(n) + n + 1 (all indices are
 *            unsigned 64‑bit integers). The function uses a lazy memoisation
 *            cache which grows as needed. It works with any seq_t that
 *            supports the usual arithmetic operators
 * Range:     u8 n <= 270, u16 n <= 9046, u32 n <= 9046, u64 n <= 9046,
 *            u128 n <= 9046, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <string.h>

/* Cache for computed values -------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;          /* array of computed values */
static size_t  cache_cap = 0;        /* current allocated capacity     */
#endif

/* Ensure the cache can hold values up to idx n */
#ifdef __cplusplus
static inline void ensure_cache(idx_t n)
{
    if (cache.size() <= n)
        cache.resize(n + 1);
}
#else
static inline void ensure_cache(idx_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);                /* abort on OOM       */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* zero the newly allocated part */
        memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache     = new_cache;
        cache_cap = new_cap;
    }
}
#endif

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise cache[0] = 0 if not already done */
    if (n == 0) {
#ifdef __cplusplus
        if (cache.empty())
            cache.resize(1, 0);
#else
        if (cache_cap == 0) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            if (!cache) exit(1);
            cache[0] = 0;
            cache_cap = 1;
        }
#endif
        return 0;
    }

    /* grow cache to accommodate n */
#ifdef __cplusplus
    size_t start = cache.size();
    ensure_cache(n);
    for (idx_t m = start; m <= n; ++m) {
        idx_t half = m >> 1;
        if (m & 1) {
            cache[m] = cache[half] + (seq_t)half + 1;
        } else {
            cache[m] = cache[half];
        }
    }
    return cache[n];
#else
    idx_t start = cache_cap;
    ensure_cache(n);
    for (idx_t m = start; m <= n; ++m) {
        idx_t half = m >> 1;
        if (m & 1) {
            cache[m] = cache[half] + (seq_t)half + 1;
        } else {
            cache[m] = cache[half];
        }
    }
    return cache[n];
#endif
}
