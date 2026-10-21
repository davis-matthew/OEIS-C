/*
 * A254847  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum one and no antidiagonal sum two.
 * https://oeis.org/A254847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the linear recurrence a(n) = a(n-1) + 8*a(n-3) +
 *            12*a(n-4) + 20*a(n-5) + 32*a(n-6) – 64*a(n-8) – 64*a(n-9) with
 *            initial values: a(1)=192, a(2)=576, a(3)=1632, a(4)=4624,
 *            a(5)=13328, a(6)=38416, a(7)=110544, a(8)=318096, a(9)=913680
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialize the cache with the first 9 terms on first call. */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)calloc(cache_len + 1, sizeof(seq_t));
        if (!cache) return 0;            /* out of memory, unlikely in tests */

        cache[1] = 192;
        cache[2] = 576;
        cache[3] = 1632;
        cache[4] = 4624;
        cache[5] = 13328;
        cache[6] = 38416;
        cache[7] = 110544;
        cache[8] = 318096;
        cache[9] = 913680;
    }

    /* If the requested n is already computed, return it. */
    if ((size_t)n <= cache_len) {
        return cache[n];
    }

    /* Grow the cache up to the requested n and fill in the missing terms. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!new_cache) return 0;             /* out of memory */
    cache = new_cache;

    for (size_t i = cache_len + 1; i <= new_len; ++i) {
        /* Compute using the recurrence.  All indices are 1‑based. */
        cache[i] = cache[i-1]
                 + 8 * cache[i-3]
                 + 12 * cache[i-4]
                 + 20 * cache[i-5]
                 + 32 * cache[i-6]
                 - 64 * cache[i-8]
                 - 64 * cache[i-9];
    }

    cache_len = new_len;
    return cache[n];
}
