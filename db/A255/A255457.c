/*
 * A255457  a(n) = A255456(2^n-1).
 * https://oeis.org/A255457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 24*a(n-3) + 15*a(n-4) + 17*a(n-5) for n>4,
 *            a(0)=1, a(1)=5, a(2)=23, a(3)=93, a(4)=359
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[5] = {1, 5, 23, 93, 359};

seq_t a(idx_t n) {
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* next index to compute */

    /* Ensure cache has enough capacity for n+1 elements */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) return 0;  /* allocation failed */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            cache = tmp;
        }
        /* Initialize first five terms if they were not already in cache */
        for (size_t i = 0; i < 5 && i < new_cap; ++i) {
            if (i >= cache_cap) cache[i] = init[i];
        }
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 4) {
                cache[i] = init[i];
            } else {
                /* Compute recurrence using only addition and one subtraction
                   to avoid unsigned underflow. */
                seq_t val = 5 * cache[i - 1];
                val += 15 * cache[i - 4];
                val += 17 * cache[i - 5];
                val -= 24 * cache[i - 3];
                cache[i] = val;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
