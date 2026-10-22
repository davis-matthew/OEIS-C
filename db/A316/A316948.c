/*
 * A316948  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 10*a(n-2) + 15*a(n-3) + 8*a(n-4) + 3*a(n-5) +
 *            a(n-6) + 2*a(n-7) with a(1)=1, a(2)=25, a(3)=160, a(4)=1240,
 *            a(5)=9417, a(6)=71505, a(7)=543155
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 1.  cache_len is the largest
       index computed so far.  cache_cap is the allocated array size. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure that the cache can hold up to index n. */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) > 16 ? (n + 1) : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    if (n > cache_len) {
        for (size_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1)       cache[i] = 1;
            else if (i == 2)  cache[i] = 25;
            else if (i == 3)  cache[i] = 160;
            else if (i == 4)  cache[i] = 1240;
            else if (i == 5)  cache[i] = 9417;
            else if (i == 6)  cache[i] = 71505;
            else if (i == 7)  cache[i] = 543155;
            else
                cache[i] = 6*cache[i-1] + 10*cache[i-2] + 15*cache[i-3]
                         + 8*cache[i-4] + 3*cache[i-5] + cache[i-6]
                         + 2*cache[i-7];
        }
        cache_len = n;
    }

    return cache[n];
}
