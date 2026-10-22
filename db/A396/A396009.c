/*
 * A396009  Number of 2's in the partitions of n into exactly 4 parts.
 * https://oeis.org/A396009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6) for n > 14
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1,
 *            a(6)=2, a(7)=4, a(8)=7, a(9)=7, a(10)=9, a(11)=11, a(12)=13,
 *            a(13)=15, a(14)=18
 * Range:     u8 n <= 54, u16 n <= 885, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[15] = {
        0, 0, 0, 0, 0,
        1, 2, 4, 7, 7,
        9, 11, 13, 15, 18
    };
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n + 1 > cache_size) {
        /* Grow cache to accommodate n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;

        /* Compute values from the first missing index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 14) {
                cache[i] = init[i];
            } else {
                /* a(i) = a(i-1) + a(i-2) - a(i-4) - a(i-5) + a(i-6) */
                seq_t val = cache[i - 1] + cache[i - 2];
                val -= cache[i - 4];
                val -= cache[i - 5];
                val += cache[i - 6];
                cache[i] = val;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
