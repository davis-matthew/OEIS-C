/*
 * A253360  Number of (n+2) X (1+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum unequal to
 *          its neighbors horizontally and vertically.
 * https://oeis.org/A253360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) - 3*a(n-4) + 4*a(n-6), n >= 7 with initial
 *            values (offset 1): a(1)=152, a(2)=272, a(3)=560, a(4)=976,
 *            a(5)=1904, a(6)=3472
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 28, u64 n <= 64, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_computed = 0;   /* largest index computed */

    /* initialize cache with necessary capacity */
    if (!cache) {
        /* start with room for first 64 indices (1‑based) */
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        /* set initial terms */
        cache[1] = (seq_t)152;
        cache[2] = (seq_t)272;
        cache[3] = (seq_t)560;
        cache[4] = (seq_t)976;
        cache[5] = (seq_t)1904;
        cache[6] = (seq_t)3472;
        cache_computed = 6;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = cache_capacity * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_computed) {
        for (size_t i = cache_computed + 1; i <= (size_t)n; ++i) {
            if (i <= 6) {
                /* already set above */
                continue;
            }
            seq_t term = cache[i - 2];
            term *= (seq_t)4;          /* 4*a(i-2) */
            seq_t sub = cache[i - 4];
            sub *= (seq_t)3;           /* 3*a(i-4) */
            term -= sub;               /* 4*a(i-2) - 3*a(i-4) */
            seq_t add = cache[i - 6];
            add *= (seq_t)4;           /* 4*a(i-6) */
            term += add;               /* + 4*a(i-6) */
            cache[i] = term;
        }
        cache_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
