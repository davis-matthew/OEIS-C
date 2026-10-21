/*
 * A266367  Expansion of b(2)*b(4)/(1 - 2*x - 2*x^3 + 3*x^4), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=10, a(3)=24, a(4)=54, a(n) = 2*a(n-1) +
 *            2*a(n-3) - 3*a(n-4) (n>4)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements O(n) computation with memoisation, suitable for
   any idx_t sequence index. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small initial values, used directly when n<5 */
    static const seq_t init[5] = {1, 4, 10, 24, 54};

    if (n < 5) {
        return init[(size_t)n];
    }

    /* On first use, allocate scratch space to memoise results.
       cache_len holds the number of computed entries (indexes 0..cache_len-1). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* size of allocated block */
    static size_t cache_len = 0;   /* highest computed index + 1 */

    /* Compute up to required index if necessary. */
    if (cache_len <= (size_t)n) {
        /* Ensure capacity at least up to n+1. */
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) {
            new_cap *= 2;
        }

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: abort rather than corrupt output. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;

        /* Initialise first 5 entries if this is the first allocation. */
        if (cache_len == 0) {
            cache[0] = 1;
            cache[1] = 4;
            cache[2] = 10;
            cache[3] = 24;
            cache[4] = 54;
            cache_len = 5;   /* we have values up to index 4 */
        }

        /* Iteratively compute missing entries up to n. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 2 * cache[i - 3] - 3 * cache[i - 4];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
