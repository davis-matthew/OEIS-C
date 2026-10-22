/*
 * A345887  Number of tilings of an n-cell circular array with rectangular
 *          tiles of any size, and where the number of possible colors of a
 *          tile is given by the largest cell covered.
 * https://oeis.org/A345887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)+(-n-2)*a(n-1)+(n-1)*a(n-2)-2=0 with initial values a(1)=1,
 *            a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) {            /* offset starts at 1, but guard for safety */
        return 0;
    }

    /* Ensure cache has space up to n, and compute missing entries */
    if (cache == NULL) {      /* first call */
        cache_size = 2;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return 0; // memory failure, return zero
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
    }
    if (n <= cache_size) {
        return cache[n];
    }

    /* Resize cache and compute missing terms */
    if (n > cache_size) {
        /* grow cache to accommodate up to n (idx_t may exceed size_t, but n is small) */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return 0;            // memory failure
        /* compute terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            /* a(i) = (i+2)*prev1 - (i-1)*prev2 + 2 */
            cache[i] = (seq_t)(i + 2) * prev1
                     - (seq_t)(i - 1) * prev2
                     + (seq_t)2;
        }
        cache_size = n;
    }

    return cache[n];
}
