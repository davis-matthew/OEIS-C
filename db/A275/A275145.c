/*
 * A275145  Number of 5Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(1) … a(12) are initial values: a(1)=41,
 *            a(2)=1296, a(3)=2000, a(4)=5248, a(5)=9744, a(6)=32384,
 *            a(7)=107472, a(8)=362176, a(9)=1220752, a(10)=4108688,
 *            a(11)=13848016, a(12)=46763600 For n>12:
 *            a(n)=8*a(n-1)-27*a(n-2)+56*a(n-3)-75*a(n-4)+61*a(n-5)
 *            -28*a(n-6)+7*a(n-7)-a(n-8)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* the cached sequence values */
static idx_t cache_size = 0;     /* number of valid entries in cache */
static idx_t cache_cap   = 0;    /* allocated capacity of cache */

/* Expand the cache to hold at least n values. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;                           /* already available */

    /* initial allocation, at least 12 for the known terms */
    if (!cache) {
        cache_cap = 16 > n ? 16 : n * 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initial 12 values (index 0 corresponds to a(1)) */
        seq_t init[12] = {
            41,          1296,        2000, 5248, 9744,
            32384,      107472,     362176, 1220752, 4108688,
            13848016,   46763600
        };
        for (idx_t i = 0; i < 12 && i < cache_cap; ++i)
            cache[i] = init[i];
        cache_size = 12;
        if (cache_size > cache_cap) {
            cache_cap = cache_size * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
    } else if (n > cache_cap) {            /* grow array to fit n */
        while (cache_cap < n)
            cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute missing terms up to n-1 */
    for (idx_t i = cache_size; i < n; ++i) {
        if (i < 12)                        /* already set */
            continue;

        /* compute using the recurrence; split into positive and negative parts */
        seq_t pos = cache[i-1] * 8;          /* 8*a(n-1) */
        pos += cache[i-3] * 56;              /* +56*a(n-3) */
        pos += cache[i-5] * 61;              /* +61*a(n-5) */
        pos += cache[i-7] * 7;               /* +7*a(n-7) */

        seq_t neg = cache[i-2] * 27;         /* 27*a(n-2) */
        neg += cache[i-4] * 75;              /* +75*a(n-4) */
        neg += cache[i-6] * 28;              /* +28*a(n-6) */
        neg += cache[i-8];                    /* +a(n-8) */

        cache[i] = pos - neg;                /* guaranteed non‑negative */
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* value for indices <1 is undefined */
    ensure_cache(n);
    return cache[n-1];
}
