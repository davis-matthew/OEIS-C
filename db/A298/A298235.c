/*
 * A298235  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+7*a(n-2)-32*a(n-3)-51*a(n-4)+118*a(n-5)+121*a(n-6)
 *            -249*a(n-7)-90*a(n-8)+292*a(n-9)-150*a(n-11)+2*a(n-12)+12*a(n-13)
 *            for n>15; Initial values: a(1)=1, a(2)=13, a(3)=22, a(4)=124,
 *            a(5)=409, a(6)=1921, a(7)=8908, a(8)=41933, a(9)=204016,
 *            a(10)=988083, a(11)=4818956, a(12)=23520912,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of allocated slots (index 0..size-1) */

    /* Need indices 0..n inclusive */
    size_t needed = (size_t)n + 1;

    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        if (cache_len < needed) {
            /* Zero newly allocated part */
            memset(tmp + cache_len, 0, (needed - cache_len) * sizeof(seq_t));
        }
        cache = tmp;
        cache_len = needed;
    }

    /* Return if already computed */
    if (cache[n] != 0) return cache[n];

    /* Initial values up to a(15) */
    static const seq_t init[16] = {
        0ULL,            /* unused index 0 */
        1ULL,            /* a(1) */
        13ULL,           /* a(2) */
        22ULL,           /* a(3) */
        124ULL,          /* a(4) */
        409ULL,          /* a(5) */
        1921ULL,         /* a(6) */
        8908ULL,         /* a(7) */
        41933ULL,        /* a(8) */
        204016ULL,       /* a(9) */
        988083ULL,       /* a(10) */
        4818956ULL,      /* a(11) */
        23520912ULL,     /* a(12) */
        114859013ULL,    /* a(13) */
        561215577ULL,    /* a(14) */
        2742234243ULL    /* a(15) */
    };

    /* Compute missing values up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;   /* already set */

        if (i <= 15) {
            cache[i] = init[i];
        } else {
            seq_t val =
                5   * cache[i-1] + 7   * cache[i-2]
              - 32  * cache[i-3] - 51  * cache[i-4]
              + 118 * cache[i-5] + 121 * cache[i-6]
              - 249 * cache[i-7] - 90  * cache[i-8]
              + 292 * cache[i-9]
              - 150 * cache[i-11] + 2   * cache[i-12]
              + 12  * cache[i-13];
            cache[i] = val;
        }
    }

    return cache[n];
}
