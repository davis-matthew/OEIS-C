/*
 * A387264  Expansion of e.g.f. exp(x^3/(1-x)^4).
 * https://oeis.org/A387264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A387264a(n) = 5·(n-1)·a(n-1) - 10·(n-2)(n-1)·a(n-2) +
 *            (n-2)(n-1)(10n-27)·a(n-3) - (n-3)(n-2)(n-1)(5n-21)·a(n-4) +
 *            (n-5)(n-4)(n-3)(n-2)(n-1)·a(n-5) a(0) = 1, a(1) = 0, a(2) = 0,
 *            a(3) = 6, a(4) = 96, a(5) = 1200 (Valid for n ≥ 6)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of elements stored (0..cache_len-1) */

    /* base values */
    if (n < 6) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)6;
            case 4: return (seq_t)96;
            case 5: return (seq_t)1200;
        }
    }

    /* ensure the cache can hold indices up to n */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)96;
        cache[5] = (seq_t)1200;
    }

    if (cache_len <= n) {
        size_t needed = n + 1;
        if (needed > cache_len) {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!cache) abort();
            cache_len = needed;
        }

        for (idx_t i = 6; i <= n; ++i) {
            seq_t term1 = (seq_t)5 * (seq_t)(i - 1) * cache[i - 1];
            seq_t term2 = (seq_t)10 * (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 2];
            seq_t term3 = (seq_t)(i - 2) * (seq_t)(i - 1) *
                          (seq_t)(10 * i - 27) * cache[i - 3];
            seq_t term4 = (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1) *
                          (seq_t)(5 * i - 21) * cache[i - 4];
            seq_t term5 = (seq_t)(i - 5) * (seq_t)(i - 4) * (seq_t)(i - 3) *
                          (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 5];

            seq_t val = term1 + term3 + term5;
            val -= term2;
            val -= term4;
            cache[i] = val;
        }
    }

    return cache[n];
}
