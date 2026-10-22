/*
 * A335155  Start with 1; if n is in the sequence, so are n+5 and 3*n.
 * https://oeis.org/A335155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n>17, with initial values:
 *            a(1)=1, a(2)=3, a(3)=6, a(4)=8, a(5)=9, a(6)=11, a(7)=13,
 *            a(8)=14, a(9)=16, a(10)=18, a(11)=19, a(12)=21, a(13)=23,
 *            a(14)=24, a(15)=26, a(16)=27, a(17)=28
 * Range:     u8 n <= 198, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Pre‑computed values for n = 1 … 17 */
    static const seq_t init[17] = {
        1, 3, 6, 8, 9, 11, 13, 14, 16, 18,
        19, 21, 23, 24, 26, 27, 28
    };

    /* Ensure cache contains at least n entries */
    if (cache_size < n) {
        /* Initialise cache with the first 17 terms */
        if (cache_size == 0) {
            cache_size = 17;
            cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
            if (!cache) abort();
            for (idx_t i = 0; i < 17; ++i)
                cache[i] = init[i];
        }

        /* Extend cache to size n and compute new terms */
        if (n > cache_size) {
            seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;

            for (idx_t i = cache_size + 1; i <= n; ++i) {
                idx_t idx = i - 1; /* 0‑based index in the array */

                /* Recurrence: a(i) = a(i-1) + a(i-4) - a(i-5) */
                cache[idx] = cache[idx - 1] + cache[idx - 4] - cache[idx - 5];
            }

            cache_size = n;
        }
    }

    return cache[n - 1];
}
