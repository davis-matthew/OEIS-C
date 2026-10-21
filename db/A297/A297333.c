/*
 * A297333  Number of n X 3 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 3*a(n-3) Initial values: a(1)=5,
 *            a(2)=22, a(3)=96
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    if (n == 0) return 0;

    /* First call: allocate and initialise the cache */
    if (!cache) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure */
        cache[0] = (seq_t)5; /* a(1) */
        cache[1] = (seq_t)22; /* a(2) */
        cache[2] = (seq_t)96; /* a(3) */
        cache_len = 3;
    }

    /* Expand the capacity if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 3;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_len)
        return cache[n-1];

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = cache[i-2] * (seq_t)5;          /* 5*a(i-1) */
        val -= cache[i-3] * (seq_t)2;              /* -2*a(i-2) */
        val -= cache[i-4] * (seq_t)3;              /* -3*a(i-3) */
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
