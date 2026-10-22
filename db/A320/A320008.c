/*
 * A320008  a(0) = 1; for n > 0, a(n) = A000120(n) * a(n-A000120(n)), where
 *          A000120(n) is the binary weight of n.
 * https://oeis.org/A320008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = w(n) * a(n-w(n)), a(0) = 1 where w(n) = A000120(n) =
 *            number of 1‑bits in the binary expansion of n
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 69, u64 n <= 133,
 *            u128 n <= 133, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;           /* array of previously computed values   */
static size_t cache_cap = 0;          /* allocated size of cache              */
static size_t cached_size = 0;        /* number of values actually computed   */

/* Ensure that the cache has capacity for at least "need+1" elements. */
static void ensure_cache_cap(size_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* If the value has already been computed, simply return it. */
    if (n < cached_size) return cache[(size_t)n];

    /* On the first call initialise a(0) = 1. */
    if (cached_size == 0) {
        ensure_cache_cap(n);
        cache[0] = (seq_t)1;
        cached_size = 1;
    }

    /* Compute all missing terms up to n. */
    ensure_cache_cap(n);
    for (idx_t i = cached_size; i <= n; ++i) {
        idx_t w = __builtin_popcountll((unsigned long long)i); /* binary weight */
        cache[(size_t)i] = (seq_t)w * cache[(size_t)(i - w)];
    }
    cached_size = (size_t)n + 1;

    return cache[(size_t)n];
}
