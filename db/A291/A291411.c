/*
 * A291411  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 2 S - S^2 + S^3.
 * https://oeis.org/A291411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + a(n-3) - 2*a(n-4) - 3*a(n-5) -
 *            a(n-6) with initial values: a(0) = 2, a(1) = 7, a(2) = 21, a(3)
 *            = 63, a(4) = 189, a(5) = 567, a(6) = 1699
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed_len = 0; /* number of values currently computed */

    /* Convert n to size_t for array indexing */
    size_t target = (size_t)n;

    /* First call: allocate and initialize */
    if (cache == NULL) {
        cache_cap = (target + 1 > 8) ? (target + 1) : 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;          /* allocation failure guard */

        /* Initial values a(0)..a(6) */
        cache[0] = 2;
        cache[1] = 7;
        cache[2] = 21;
        cache[3] = 63;
        cache[4] = 189;
        cache[5] = 567;
        cache[6] = 1699;

        /* Compute further terms up to target if needed */
        for (size_t i = 7; i <= target; ++i) {
            cache[i] = 2 * cache[i-1] + 3 * cache[i-2] + cache[i-3]
                     - 2 * cache[i-4] - 3 * cache[i-5] - cache[i-6];
        }
        computed_len = target + 1;
        return cache[target];
    }

    /* Ensure cache has enough capacity */
    if (target >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;         /* allocation failure guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to target */
    if (target >= computed_len) {
        /* Compute from the first term that is not yet computed */
        size_t start = computed_len;
        for (size_t i = start; i <= target; ++i) {
            /* For i < 7 the values are already initialized */
            if (i >= 7) {
                cache[i] = 2 * cache[i-1] + 3 * cache[i-2] + cache[i-3]
                         - 2 * cache[i-4] - 3 * cache[i-5] - cache[i-6];
            }
        }
        computed_len = target + 1;
    }

    return cache[target];
}
