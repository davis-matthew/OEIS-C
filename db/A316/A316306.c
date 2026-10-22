/*
 * A316306  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 16‑term linear recurrence a(n) = 4 a(n-1) -4
 *            a(n-2) +5 a(n-3) -16 a(n-4) +12 a(n-5) -7 a(n-6) +18 a(n-7) -14
 *            a(n-8) +12 a(n-9) +2 a(n-10) +11 a(n-11) -13 a(n-12) -10 a(n-13)
 *            -5 a(n-14) -2 a(n-15) +4 a(n-16) with initial values: a(1)=3,
 *            a(2)=7, a(3)=10, a(4)=21, a(5)=44, a(6)=83...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;

    /* initialize cache with first 16 values if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((16 + 1) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;           /* out of memory: return 0 as sentinel */
        cache[0] = (seq_t)0;           /* unused */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)44;
        cache[6] = (seq_t)83;
        cache[7] = (seq_t)168;
        cache[8] = (seq_t)365;
        cache[9] = (seq_t)766;
        cache[10] = (seq_t)1615;
        cache[11] = (seq_t)3490;
        cache[12] = (seq_t)7555;
        cache[13] = (seq_t)16450;
        cache[14] = (seq_t)36279;
        cache[15] = (seq_t)80602;
        cache[16] = (seq_t)180187;
        cache_n = 16;
    }

    /* Return value if already computed */
    if (n <= cache_n)
        return cache[n];

    /* Grow cache up to needed index */
    if (cache == NULL)   /* should never happen */
        return (seq_t)0;
    if (n > cache_n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;               /* out of memory */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            /* Rely on already computed previous 16 terms */
            seq_t val =
                4 * cache[i - 1]
                - 4 * cache[i - 2]
                + 5 * cache[i - 3]
                - 16 * cache[i - 4]
                + 12 * cache[i - 5]
                - 7 * cache[i - 6]
                + 18 * cache[i - 7]
                - 14 * cache[i - 8]
                + 12 * cache[i - 9]
                + 2 * cache[i - 10]
                + 11 * cache[i - 11]
                - 13 * cache[i - 12]
                - 10 * cache[i - 13]
                - 5 * cache[i - 14]
                - 2 * cache[i - 15]
                + 4 * cache[i - 16];
            cache[i] = val;
        }
        cache_n = n;
    }

    return cache[n];
}
