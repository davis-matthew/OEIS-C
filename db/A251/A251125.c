/*
 * A251125  Number of (n+1) X (5+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) (n>6),
 *            a(1)=125, a(2)=193, a(3)=272, a(4)=401, a(5)=580, a(6)=854
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 177,
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
    size_t i, needed;

    if (n == 0) return 0;

    if (cache_len < 6) {
        needed = 6;
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_len = needed;
        cache[0] = 125;
        cache[1] = 193;
        cache[2] = 272;
        cache[3] = 401;
        cache[4] = 580;
        cache[5] = 854;
    }

    /* Expand cache if necessary and compute missing terms */
    if ((idx_t)cache_len < n) {
        needed = (size_t)n;
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        for (i = cache_len; i < needed; ++i) {
            /* i is 0‑based index; a(i+1) is stored at cache[i] */
            cache[i] = 4 * cache[i - 1]
                     - 5 * cache[i - 2]
                     +  cache[i - 3]
                     + 2 * cache[i - 4]
                     -  cache[i - 5];
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
