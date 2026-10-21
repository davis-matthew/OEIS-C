/*
 * A299677  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..18) as listed in OEIS For n>17: a(n) = 4*a(n-1) -2*a(n-2)
 *            +4*a(n-3) -16*a(n-4) -13*a(n-5) +12*a(n-6) +40*a(n-7) +23*a(n-8)
 *            -37*a(n-9) +15*a(n-10) -88*a(n-11) +52*a(n-12) -48*a(n-13)
 *            +49*a(n-14) -8*a(n-15) +20*a(n-16)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache (indices 0..cap-1) */
    static size_t maxc = 0;    /* highest n computed so far */

    /* initialise cache with first 18 terms */
    if (cache == NULL) {
        /* allocate space for indices 0..18 (19 elements) */
        cap = 19;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – unlikely in judge */
        /* initialise all to zero */
        for (size_t i = 0; i < cap; i++) cache[i] = 0;

        /* a[0] unused, set to 0 */
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 13;
        cache[3] = 20;
        cache[4] = 45;
        cache[5] = 147;
        cache[6] = 506;
        cache[7] = 1561;
        cache[8] = 5017;
        cache[9] = 16429;
        cache[10] = 53365;
        cache[11] = 172859;
        cache[12] = 562276;
        cache[13] = 1828820;
        cache[14] = 5943264;
        cache[15] = 19318649;
        cache[16] = 62811656;
        cache[17] = 204197935;
        cache[18] = 663816782;

        maxc = 18;
    }

    if (n < 1) return 0;   /* sequence only defined for n>=1 */

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = n + 1;
        if (newcap < cap * 2) newcap = cap * 2;   /* simple growth factor */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* memory failure – unlikely in judge */
        cache = tmp;
        /* initialise new part to zero (not strictly needed) */
        for (size_t i = cap; i < newcap; i++) cache[i] = 0;
        cap = newcap;
    }

    /* if already computed, return */
    if ((size_t)n <= maxc) return cache[n];

    /* recurrence coefficients for indices 1..16 */
    static const int coeff[16] = {
        4, -2, 4, -16, -13, 12, 40, 23,
        -37, 15, -88, 52, -48, 49, -8, 20
    };

    /* compute missing values up to n */
    for (size_t i = maxc + 1; i <= (size_t)n; i++) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 16; k++) {
            seq_t val = cache[i - (k + 1)];
            int c = coeff[k];
            if (c > 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }
        cache[i] = pos - neg;
    }
    maxc = (size_t)n;
    return cache[n];
}
