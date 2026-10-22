/*
 * A327610  Number of length n reversible string structures that are not
 *          palindromic using exactly three different colors.
 * https://oeis.org/A327610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A056327 - A000392(ceil(n/2)), computed by the linear
 *            recurrence a(1)=0, a(2)=0, a(3)=1, a(4)=4, a(5)=14, a(6)=49,
 *            a(7)=154 a(n) = 6*a(n-1) - 6*a(n-2) - 24*a(n-3) + 49*a(n-4) +
 *            6*a(n-5) - 66*a(n-6) + 36*a(n-7) for n > 7
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* values below the offset are undefined */

    static seq_t *cache = (seq_t *)0;
    static idx_t cache_max = 0;
    static size_t cache_size = 0;

    /* allocate or grow cache if necessary */
    if (!cache || n >= cache_size) {
        size_t new_size = (n + 1 > 8) ? (size_t)(n + 1) : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* memory allocation failure */
        cache = tmp;
        cache_size = new_size;

        /* initialize base values (only once) */
        if (cache_max == 0) {
            cache[0] = 0;
            cache[1] = 0;
            cache[2] = 0;
            cache[3] = 1;
            cache[4] = 4;
            cache[5] = 14;
            cache[6] = 49;
            cache[7] = 154;
            cache_max = 7;
        }
    }

    if (n <= cache_max) return cache[n];

    /* compute all missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k <= 7) continue;   /* base values already set */
        seq_t val = 6 * cache[k-1] - 6 * cache[k-2] - 24 * cache[k-3]
                    + 49 * cache[k-4] + 6 * cache[k-5] - 66 * cache[k-6]
                    + 36 * cache[k-7];
        cache[k] = val;
    }
    cache_max = n;
    return cache[n];
}
