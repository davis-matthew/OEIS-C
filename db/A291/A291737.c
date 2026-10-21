/*
 * A291737  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - S - S^2 - S^3.
 * https://oeis.org/A291737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=11, a(4)=25, a(5)=54, a(6)=121,
 *            a(7)=267, a(8)=591, a(9)=1310. For n >= 10:
 *            a(n)=a(n-1)+a(n-2)+2*a(n-3)+2*a(n-4)+3*a(n-5)+a(n-6)+3*a(n-7)+a(n-9)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialize cache with the first 10 terms if not already done */
    if (cache_size == 0) {
        cache_size = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;                /* fail gracefully */
        cache[0] = 1;  /* a(0) */
        cache[1] = 2;  /* a(1) */
        cache[2] = 5;  /* a(2) */
        cache[3] = 11; /* a(3) */
        cache[4] = 25; /* a(4) */
        cache[5] = 54; /* a(5) */
        cache[6] = 121;/* a(6) */
        cache[7] = 267;/* a(7) */
        cache[8] = 591;/* a(8) */
        cache[9] = 1310;/* a(9) */
    }

    /* If the requested index is already in the cache, return it */
    if ((idx_t)n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to accommodate indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0; /* allocation failed: return zero */
    cache = new_cache;

    /* Compute any missing terms up to n using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        idx_t idx = (idx_t)i;
        seq_t val = cache[idx-1] + cache[idx-2]       /* a(n-1)+a(n-2) */
                     + cache[idx-3] * 2              /* +2*a(n-3)   */
                     + cache[idx-4] * 2              /* +2*a(n-4)   */
                     + cache[idx-5] * 3              /* +3*a(n-5)   */
                     + cache[idx-6]                  /* +a(n-6)     */
                     + cache[idx-7] * 3              /* +3*a(n-7)   */
                     + cache[idx-9];                 /* +a(n-9)     */
        cache[idx] = val;
    }

    cache_size = new_size;
    return cache[n];
}
