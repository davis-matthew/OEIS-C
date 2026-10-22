/*
 * A333897  Number of unlabeled loopless multigraphs with 4 nodes of degree n
 *          or less.
 * https://oeis.org/A333897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-4*a(n-3)-4*a(n-4)+2*a(n-5)+9*a(n-6)
 *            -9*a(n-8)-2*a(n-9)+4*a(n-10)+4*a(n-11)-2*a(n-12)-2*a(n-13)+a(n-14)
 *            for n>13; initial values:
 *            a(0)=1,a(1)=3,a(2)=10,a(3)=26,a(4)=61,a(5)=128,
 *            a(6)=254,a(7)=467,a(8)=821,a(9)=1376,a(10)=2224,a(11)=3470,a(12)=5269,
 *            a(13)=7785,a(14)=11254
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure capacity up to n */
    if (n >= cache_len) {
        if (cache_cap < (size_t)(n + 1)) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Initial terms */
        static const seq_t init[15] = {
            1, 3, 10, 26, 61, 128, 254, 467, 821, 1376,
            2224, 3470, 5269, 7785, 11254
        };

        size_t i;
        for (i = cache_len; i <= (size_t)n; ++i) {
            if (i < 15) {
                cache[i] = init[i];
            } else {
                cache[i] =
                    2 * cache[i - 1] + 2 * cache[i - 2]
                    - 4 * cache[i - 3] - 4 * cache[i - 4]
                    + 2 * cache[i - 5] + 9 * cache[i - 6]
                    - 9 * cache[i - 8] - 2 * cache[i - 9]
                    + 4 * cache[i -10] + 4 * cache[i -11]
                    - 2 * cache[i -12] - 2 * cache[i -13]
                    + cache[i -14];
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
