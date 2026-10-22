/*
 * A387602  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 2^(n-2*k) *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A387602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=120, a(5)=416
   a(n)=4*a(n-1)-4*a(n-2)+4*a(n-3)+8*a(n-4)-4*a(n-6)  (n≥6) */

#include <stdlib.h>

static const seq_t init[6] = { 1, 4, 12, 36, 120, 416 };
static seq_t *cache = NULL;          /* dynamically growing cache */
static size_t cache_len = 0;          /* number of filled entries in cache */
static size_t cache_cap = 0;          /* current allocated capacity */

/* Grow the cache array so that it can hold at least 'new_len' elements. */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that cache contains values up to at least n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len) return;

    /* If the cache is empty, prime it with the initial 6 values. */
    if (cache_len == 0) {
        grow_cache(6);
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 6) {
            val = init[(size_t)i];
        } else {
            val = 4 * cache[i - 1]      /* 4*a(i-1) */
                - 4 * cache[i - 2]      /*  -4*a(i-2) */
                + 4 * cache[i - 3]      /*  +4*a(i-3) */
                + 8 * cache[i - 4]      /*  +8*a(i-4) */
                - 4 * cache[i - 6];     /*  -4*a(i-6) */
        }
        if (i >= cache_cap) grow_cache(i + 1);
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
