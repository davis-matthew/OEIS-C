/*
 * A248062  Number of length n+5 0..2 arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 37, u64 n <= 85, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 208 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A248062
 * Formula: a(n) = a(n-1) + 20*a(n-6) - 20*a(n-7) - 64*a(n-12) + 64*a(n-13)
 * Offset: 1 (a(1) = 333)
 * Initial values a(1) … a(13) are hard‑coded.
 */
#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

const seq_t A248062_seed[14] = {
    0,          /* unused */
    333,        /* a(1) */
    513,        /* a(2) */
    793,        /* a(3) */
    1221,       /* a(4) */
    1861,       /* a(5) */
    2793,       /* a(6) */
    4113,       /* a(7) */
    6753,       /* a(8) */
    10945,      /* a(9) */
    17457,      /* a(10) */
    27313,      /* a(11) */
    41793,      /* a(12) */
    62433       /* a(13) */
};

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* first index that is not yet stored (0‑based) */

    /* initialise cache with the 13 seed values */
    if (cache == NULL) {
        cache_len = 14;                       /* store indices 0..13 inclusive */
        cache = mem_new(seq_t, cache_len);
        for (size_t i = 0; i < 14; ++i)
            cache[i] = A248062_seed[i];
    }

    /* If requested value is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate up to n */
    size_t new_len = (size_t)n + 1;
    cache = mem_resize(seq_t, cache, new_len);

    /* Compute missing terms sequentially up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = A248062_seed[i];
            continue;
        }
        /* Use unsigned arithmetic only – the addition order guarantees no underflow */
        seq_t t1 = cache[i-1];
        seq_t t2 = mul_small(cache[i-6], 20);
        seq_t t3 = mul_small(cache[i-7], 20);
        seq_t t4 = mul_small(cache[i-12], 64);
        seq_t t5 = mul_small(cache[i-13], 64);

        seq_t sum1 = t1 + t2 + t5;               /* a(n-1) + 20*a(n-6) + 64*a(n-13) */
        seq_t sum2 = t3 + t4;                   /* 20*a(n-7) + 64*a(n-12) */
        cache[i] = sum1 - sum2;                 /* difference is non‑negative */
    }
    cache_len = new_len;
    return cache[n];
}
