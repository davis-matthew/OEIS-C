/*
 * A279439  Number of ways to place 5 points on an n X n square grid so that
 *          no more than 2 points are on a vertical or horizontal straight
 *          line.
 * https://oeis.org/A279439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 -30*n^8 +90*n^7 -27*n^6 -270*n^5 +500*n^4 -360*n^3
 *            +96*n^2)/120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Cache previous terms so that subsequent calls are O(1). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* highest n for which cache[n-1] is valid */

    if (n == 0) return (seq_t)0;        /* offset starts at 1 */

    /* Grow cache if necessary and compute any missing terms. */
    if (n > cache_size) {
        /* allocate space for all terms up to n */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));

        /* base values for n = 1..11 */
        static const seq_t base[11] = {
            (seq_t)0,
            (seq_t)0,
            (seq_t)45,
            (seq_t)2304,
            (seq_t)34020,
            (seq_t)270720,
            (seq_t)1475145,
            (seq_t)6209280,
            (seq_t)21654864,
            (seq_t)65422080,
            (seq_t)176467005
        };

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i-1] = base[i-1];
            } else {
                /* recurrence:
                   a(n) = 11*a(n-1) -55*a(n-2) +165*a(n-3) -330*a(n-4) +462*a(n-5)
                          -462*a(n-6) +330*a(n-7) -165*a(n-8) +55*a(n-9)
                          -11*a(n-10) + a(n-11)
                */
                seq_t val = 0;
                val += (seq_t)11 *  cache[i-2];
                val -= (seq_t)55 *  cache[i-3];
                val += (seq_t)165 * cache[i-4];
                val -= (seq_t)330 * cache[i-5];
                val += (seq_t)462 * cache[i-6];
                val -= (seq_t)462 * cache[i-7];
                val += (seq_t)330 * cache[i-8];
                val -= (seq_t)165 * cache[i-9];
                val += (seq_t)55 *  cache[i-10];
                val -= (seq_t)11 *  cache[i-11];
                val +=          cache[i-12];
                cache[i-1] = val;
            }
        }
        cache_size = n;
    }
    return cache[n-1];
}
