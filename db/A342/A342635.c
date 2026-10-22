/*
 * A342635  a(0) = 0, a(1) = 1; a(2*n) = a(n), a(2*n+1) = 5*a(n) + a(n+1).
 * https://oeis.org/A342635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = a(n); a(2*n+1) = 5*a(n) + a(n+1)
 * Range:     u8 n <= 22, u16 n <= 254, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;      /* memoised values */
static uint8_t *used  = NULL;      /* flag: 0 = unknown, 1 = computed */
static size_t cache_cap = 0;

/* Ensure the cache is large enough to hold index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *t = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!t) abort();
        cache = t;

        uint8_t *u = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
        if (!u) abort();
        used = u;
        for (size_t i = cache_cap; i < new_cap; ++i) used[i] = 0;
        cache_cap = new_cap;
    }
}

/* Recursive helper with memoisation. */
static seq_t rec(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    ensure_cache(n);
    if (used[n]) return cache[n];

    seq_t val;
    if ((n & 1ULL) == 0) {
        /* a(2*n) = a(n) */
        val = rec(n >> 1);
    } else {
        /* a(2*n+1) = 5*a(n) + a(n+1) */
        val = 5 * rec(n >> 1) + rec((n >> 1) + 1);
    }

    cache[n]  = val;
    used[n]   = 1;
    return val;
}

seq_t a(idx_t n)
{
    return rec(n);
}
