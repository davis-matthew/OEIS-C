/*
 * A284700  Number of edge covers in the n-antiprism graph.
 * https://oeis.org/A284700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1)+18*a(n-2)+a(n-3)-4*a(n-4) a(0) = 4, a(1) = 13,
 *            a(2) = 205, a(3) = 2902
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        cache[0] = 4;
        cache[1] = 13;
        cache[2] = 205;
        cache[3] = 2902;
        cap = 4;
        initialized = 1;
    }

    if ((size_t)n < cap) {
        return cache[n];
    }

    /* Expand cache up to n */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; return zero or abort as appropriate */
        return 0;
    }
    cache = new_cache;

    for (idx_t i = (idx_t)cap; i <= n; ++i) {
        cache[i] = 13 * cache[i-1]
                 + 18 * cache[i-2]
                 +   cache[i-3]
                 -  4 * cache[i-4];
    }
    cap = new_cap;
    return cache[n];
}
