/*
 * A291224  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)^4.
 * https://oeis.org/A291224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 5*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) - 4*a(n-7) - a(n-8) a(0) = 4 a(1) = 10 a(2) = 24 a(3) =
 *            55 a(4) = 120 a(5) = 254 a(6) = 524 a(7) = 1059 a(8) = 2104
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 32, u64 n <= 74, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper that multiplies by a small constant without overflow for 64‑bit seq_t */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned __int128)x * m);
    }
#endif
    return (seq_t)m * x;
}

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of elements stored */

    /* Initialize cache with first 9 terms */
    if (cache == NULL) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed */
        cache[0] = 4;
        cache[1] = 10;
        cache[2] = 24;
        cache[3] = 55;
        cache[4] = 120;
        cache[5] = 254;
        cache[6] = 524;
        cache[7] = 1059;
        cache[8] = 2104;
    }

    /* Return cached value if available */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow cache to at least n+1 elements */
    size_t new_size = cache_size;
    while (new_size <= (size_t)n) {
        new_size <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;               /* reallocation failed */
    cache = tmp;

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term =
            mul_small(cache[i-1], 4)
            - mul_small(cache[i-2], 2)
            - mul_small(cache[i-3], 8)
            + mul_small(cache[i-4], 5)
            + mul_small(cache[i-5], 8)
            - mul_small(cache[i-6], 2)
            - mul_small(cache[i-7], 4)
            - cache[i-8];
        cache[i] = term;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
