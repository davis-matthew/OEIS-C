/*
 * A298186  Number of nX7 0..1 arrays with every element equal to 3, 5, 7 or 8
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -2*a(n-2) -a(n-3) +2*a(n-4) -2*a(n-5) +a(n-6)
 *            -a(n-7) +a(n-8) -a(n-10) +a(n-11) for n>12; Initial values:
 *            a(1)=0, a(2)=8, a(3)=8, a(4)=9, a(5)=10, a(6)=14, a(7)=19,
 *            a(8)=27, a(9)=38, a(10)=58, a(11)=90, a(12)=142
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 89, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    static idx_t maxComputed = 0;

    if (n < 1) return 0;

    /* initialise cache with base values up to n=12 */
    if (cache == NULL) {
        cacheSize = 13;                 /* indices 0 .. 12 */
        cache = (seq_t*)calloc(cacheSize, sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation error */

        /* assign base values */
        cache[1] = 0;
        cache[2] = 8;
        cache[3] = 8;
        cache[4] = 9;
        cache[5] = 10;
        cache[6] = 14;
        cache[7] = 19;
        cache[8] = 27;
        cache[9] = 38;
        cache[10] = 58;
        cache[11] = 90;
        cache[12] = 142;
        maxComputed = 12;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cacheSize) {
        size_t newSize = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newSize * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        for (size_t i = cacheSize; i < newSize; ++i) cache[i] = 0;
        cacheSize = newSize;
    }

    /* compute missing values up to n */
    if (n > maxComputed) {
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            if (i <= 12) continue;   /* already initialised */

            seq_t t = 3 * cache[i-1];
            t -= 2 * cache[i-2];
            t -= cache[i-3];
            t += 2 * cache[i-4];
            t -= 2 * cache[i-5];
            t += cache[i-6];
            t -= cache[i-7];
            t += cache[i-8];
            t -= cache[i-10];
            t += cache[i-11];

            cache[i] = t;
        }
        maxComputed = n;
    }

    return cache[n];
}
