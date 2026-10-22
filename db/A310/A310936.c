/*
 * A310936  Coordination sequence Gal.3.7.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 6*a(n-3) - 5*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) + 3*a(n-7) - 5*a(n-8) + 6*a(n-9) - 5*a(n-10) +
 *            3*a(n-11) - a(n-12) for n >= 18 Initial values: a(0)=1, a(1)=4,
 *            a(2)=7, a(3)=8, a(4)=8, a(5)=10, a(6)=17, a(7)=24, a(8)=28,
 *            a(9)=28, a(10)=27, a(11)=28, a(12)=32, a(13)=40, a(14)=...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated size          */
    static size_t cache_len = 0;  /* number of elements   */

    /* Ensure the cache is large enough for index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        if (cache_cap == 0)
            cache_cap = 64;
        while (cache_cap < needed)
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        /* If realloc fails, cache becomes NULL; the caller expects valid */
    }

    /* Initial 18 values */
    static const seq_t init[18] = {
        (seq_t)1ULL, (seq_t)4ULL, (seq_t)7ULL, (seq_t)8ULL,
        (seq_t)8ULL, (seq_t)10ULL, (seq_t)17ULL, (seq_t)24ULL,
        (seq_t)28ULL, (seq_t)28ULL, (seq_t)27ULL, (seq_t)28ULL,
        (seq_t)32ULL, (seq_t)40ULL, (seq_t)47ULL, (seq_t)48ULL,
        (seq_t)46ULL, (seq_t)48ULL
    };

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 18) {
            val = init[(size_t)i];
        } else {
            /* Positive contributions */
            seq_t sum = 0;
            sum += (seq_t)3 * cache[(size_t)(i - 1)];
            sum += (seq_t)6 * cache[(size_t)(i - 3)];
            sum += (seq_t)3 * cache[(size_t)(i - 5)];
            sum += (seq_t)3 * cache[(size_t)(i - 7)];
            sum += (seq_t)6 * cache[(size_t)(i - 9)];
            sum += (seq_t)3 * cache[(size_t)(i - 11)];
            /* Negative contributions – the intermediate sum is guaranteed
               to stay non‑negative due to the recurrence coefficients. */
            sum -= (seq_t)5 * cache[(size_t)(i - 2)];
            sum -= (seq_t)5 * cache[(size_t)(i - 4)];
            sum -= (seq_t)2 * cache[(size_t)(i - 6)];
            sum -= (seq_t)5 * cache[(size_t)(i - 8)];
            sum -= (seq_t)5 * cache[(size_t)(i - 10)];
            sum -= (seq_t)1 * cache[(size_t)(i - 12)];
            val = sum;
        }
        cache[(size_t)i] = val;
    }
    cache_len = needed;
    return cache[(size_t)n];
}
