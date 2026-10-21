/*
 * A295247  Number of n X 2 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 3 1s.
 * https://oeis.org/A295247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + a(n-3) – a(n-5) (with a(1)=4,
 *            a(2)=11, a(3)=35, a(4)=106, a(5)=327)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache is zero‑based: cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index stored (1‑based) */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n < 1)                 /* undefined for n < 1, return 0 */
        return 0;

    /* Ensure enough capacity: at least max(n,5) to hold the required terms */
    size_t need = (size_t)n;
    if (need < 5) need = 5;
    if (need > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache)             /* OOM – abort quietly */
            return 0;
        cache = new_cache;
        cache_cap = need;
    }

    /* Initialise the first five terms once */
    if (cache_len < 5) {
        cache[0] = 4;    /* a(1) */
        cache[1] = 11;   /* a(2) */
        cache[2] = 35;   /* a(3) */
        cache[3] = 106;  /* a(4) */
        cache[4] = 327;  /* a(5) */
        cache_len = 5;
    }

    /* Compute any missing terms up to n */
    for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
        seq_t term = 2 * cache[idx - 2]          /* 2*a(n-1) */
                   + 3 * cache[idx - 3]          /* 3*a(n-2) */
                   +      cache[idx - 4];        /* a(n-3) */
        if (idx >= 6)               /* subtract a(n-5) */
            term -= cache[idx - 6];
        cache[idx - 1] = term;      /* store a(idx) */
    }

    if (cache_len < (size_t)n)
        cache_len = (size_t)n;  /* keep cache_len large enough */

    return cache[(size_t)n - 1];
}
