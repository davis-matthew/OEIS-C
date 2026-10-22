/*
 * A322780  First differences of A237262.
 * https://oeis.org/A322780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - a(n-4) for n>4 with a(1)=1, a(2)=4, a(3)=9,
 *            a(4)=32 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;          /* array holding terms 1..n */
static size_t cache_cap = 0;         /* allocated size of cache */
static size_t cache_filled = 0;      /* highest index (1‑based) that has been filled */

/* compute the n-th term (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* handle the first four terms directly */
    if (n <= 4) {
        switch (n) {
            case 1: return 1;
            case 2: return 4;
            case 3: return 9;
            case 4: return 32;
            default: return 0;      /* unreachable */
        }
    }

    /* allocate or grow the cache to hold the first n terms */
    if (cache == NULL) {
        cache_cap = n;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < (size_t)n) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialise the first four values if they are not already set */
    if (cache_filled < 4) {
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 9;
        cache[3] = 32;
        cache_filled = 4;
    }

    /* compute missing values using the recurrence
       cache[i] corresponds to a(i+1); for i>=4 we have
       a(i+1) = 8*a(i-1) - a(i-3)  →  cache[i] = 8*cache[i-2] - cache[i-4] */
    if ((size_t)n > cache_filled) {
        for (size_t i = cache_filled; i < (size_t)n; ++i) {
            cache[i] = (seq_t)8 * cache[i-2] - cache[i-4];
        }
        cache_filled = (size_t)n;
    }

    return cache[(size_t)(n) - 1];
}
