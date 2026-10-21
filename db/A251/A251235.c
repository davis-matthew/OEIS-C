/*
 * A251235  Number of (n+1) X (7+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 63*a(n-2) + 122*a(n-3) + 13*a(n-4) -
 *            371*a(n-5) + 317*a(n-6) + 308*a(n-7) - 370*a(n-8) - 144*a(n-9) +
 *            128*a(n-10) + 48*a(n-11) Initial terms: a(1)=27714, a(2)=42850,
 *            a(3)=71322, a(4)=129378, a(5)=250622, a(6)=514950, a(7)=1112526,
 *            a(8)=2511890, a(9)=5892002, a(10)=14279706, a(1...
 * Range:     u8 none, u16 n <= 2, u32 n <= 15, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* memoisation cache: cache[i] holds a(i), i >= 1 */
    static seq_t *cache = NULL;   /* holds a(0) to a(cap) */
    static size_t cap   = 0;      /* highest index stored */

    /* a(1) .. a(11) */
    static const seq_t init[12] = {
        0ULL,          /* a(0) unused */
        27714ULL,      /* a(1) */
        42850ULL,      /* a(2) */
        71322ULL,      /* a(3) */
        129378ULL,     /* a(4) */
        250622ULL,     /* a(5) */
        514950ULL,     /* a(6) */
        1112526ULL,    /* a(7) */
        2511890ULL,    /* a(8) */
        5892002ULL,    /* a(9) */
        14279706ULL,   /* a(10) */
        35575142ULL    /* a(11) */
    };

    if (n == 0) {
        return (seq_t)0;   /* undefined for n<1, return 0 */
    }

    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;                     /* need indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) {                               /* allocation failed */
            return (seq_t)0;
        }
        cache = new_cache;

        /* initialise already known initial values */
        size_t start = (cap == 0) ? 1 : cap + 1;
        for (size_t i = start; i <= new_cap; ++i) {
            if (i <= 11) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(n) = 13*a(n-1) - 63*a(n-2) + 122*a(n-3) + 13*a(n-4)
                 *            - 371*a(n-5) + 317*a(n-6) + 308*a(n-7) - 370*a(n-8)
                 *            - 144*a(n-9) + 128*a(n-10) + 48*a(n-11) */
                seq_t v = (seq_t)13 * cache[i - 1];
                v += (seq_t)122 * cache[i - 3];
                v += (seq_t)13  * cache[i - 4];
                v += (seq_t)317 * cache[i - 6];
                v += (seq_t)308 * cache[i - 7];
                v += (seq_t)128 * cache[i -10];
                v += (seq_t)48  * cache[i -11];
                v -= (seq_t)63  * cache[i - 2];
                v -= (seq_t)371 * cache[i - 5];
                v -= (seq_t)370 * cache[i - 8];
                v -= (seq_t)144 * cache[i - 9];
                cache[i] = v;
            }
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
