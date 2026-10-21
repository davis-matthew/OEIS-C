/*
 * A246643  A sequence used in the touching circle problem described in
 *          A247512.
 * https://oeis.org/A246643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 10, a(2) = 121
   a(n) = 31*a(n-1) - 279*a(n-2) + 729*a(n-3),  for n >= 3
*/

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of elements allocated */
static idx_t  cache_len  = 0; /* next unused index (count of stored values) */

/* Ensure cache contains a(i) for all i <= idx. */
static void ensure_cache (idx_t idx)
{
    if (cache == NULL) {
        /* Allocate space for the first three terms. */
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { exit(EXIT_FAILURE); }
        cache[0] = 1;
        cache[1] = 10;
        cache[2] = 121;
        cache_len = 3;
    }

    if (idx < cache_len)
        return;

    /* Grow the allocation to hold up to idx. */
    size_t needed_cap = cache_len;
    while (needed_cap <= (size_t)idx)
        needed_cap <<= 1;

    if (needed_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) { free(cache); exit(EXIT_FAILURE); }
        cache = tmp;
        cache_cap = needed_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len; i <= idx; ++i) {
        seq_t v1 = cache[i-1] * 31;
        seq_t v2 = cache[i-2] * 279;
        seq_t v3 = cache[i-3] * 729;
        seq_t val = v1 + v3; /* v1 - v2 + v3, but compute as (v1+v3)-v2 to avoid underflow */
        val -= v2;
        cache[i] = val;
    }
    cache_len = idx + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
