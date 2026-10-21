/*
 * A282642  Number of n X 3 0..1 arrays with no 1 equal to more than one of
 *          its king-move neighbors.
 * https://oeis.org/A282642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) + 8*a(n-3) - 5*a(n-4) + 2*a(n-5) -
 *            2*a(n-6), with a(1)=7, a(2)=27, a(3)=99, a(4)=413, a(5)=1601,
 *            a(6)=6349
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of stored terms */
    static size_t cache_capacity = 0;  /* allocated capacity */

    /* Return small values directly (no need to allocate cache). */
    if (n <= 6) {
        static const seq_t init[6] = {
            (seq_t)7,
            (seq_t)27,
            (seq_t)99,
            (seq_t)413,
            (seq_t)1601,
            (seq_t)6349
        };
        return init[n-1];
    }

    /* If we've already computed up to n, just return it. */
    if (cache && n <= cache_size) {
        return cache[n-1];
    }

    /* Ensure we have storage for all required terms. */
    if (!cache) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
    } else if (n > cache_capacity) {
        /* Grow to at least n, doubling each time. */
        while (cache_capacity < n) {
            cache_capacity <<= 1;
        }
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_capacity);
    }

    /* If cache was just allocated, initialize the first 6 terms. */
    if (cache_size == 0) {
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)99;
        cache[3] = (seq_t)413;
        cache[4] = (seq_t)1601;
        cache[5] = (seq_t)6349;
        cache_size = 6;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* indices into cache (zero‑based):
           a(i-1) -> cache[i-2]
           a(i-2) -> cache[i-3]
           a(i-3) -> cache[i-4]
           a(i-4) -> cache[i-5]
           a(i-5) -> cache[i-6]
           a(i-6) -> cache[i-7]        */
        seq_t pos = 2*cache[i-2] + 6*cache[i-3] + 8*cache[i-4] + 2*cache[i-6];
        seq_t neg = 5*cache[i-5] + 2*cache[i-7];
        cache[i-1] = pos - neg;  /* subtraction is safe because pos ≥ neg */
    }

    cache_size = n;
    return cache[n-1];
}
