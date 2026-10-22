/*
 * A328881  a(n+3) = 2^n - a(n), a(0)=a(2)=1, a(1)=0 for n >= 0.
 * https://oeis.org/A328881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 2^n - a(n) with a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized sequence values   */
    static size_t len   = 0;       /* current number of cached terms */

    /* Grow the cache until it contains element n */
    if (len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        if (new_len < 3) new_len = 3;          /* at least three for the initial values */

        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failed */

        /* If this is the first allocation, initialise the three known terms */
        if (len == 0) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)0;
            new_cache[2] = (seq_t)1;
        }

        /* Compute missing terms from the current length up to n */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 3) continue;              /* already initialised */
            /* a(i) = 2^(i-3) - a(i-3) */
            new_cache[i] = pow2((idx_t)(i - 3)) - new_cache[i - 3];
        }

        cache = new_cache;
        len   = new_len;
    }
    return cache[n];
}
