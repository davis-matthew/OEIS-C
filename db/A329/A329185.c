/*
 * A329185  Number of ways to tile a 2 X n grid with dominoes and L-trominoes
 *          such that no four tiles meet at a corner.
 * https://oeis.org/A329185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - a(n-4) + 2*a(n-5), with
 *            initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=10
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache to hold indices up to n. */
    if (n + 1 > cache_size) {
        idx_t const new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed – should not happen */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute a(0) … a(n), overriding earlier contents if any. */
    for (idx_t i = 0; i <= n; ++i) {
        switch (i) {
            case 0:  cache[i] = 1; break;
            case 1:  cache[i] = 1; break;
            case 2:  cache[i] = 2; break;
            case 3:  cache[i] = 5; break;
            case 4:  cache[i] = 10; break;
            default: cache[i] = cache[i-1] * ((seq_t)2)
                                 - cache[i-2]
                                 + cache[i-3] * ((seq_t)3)
                                 - cache[i-4]
                                 + cache[i-5] * ((seq_t)2);
        }
    }

    return cache[n];
}
