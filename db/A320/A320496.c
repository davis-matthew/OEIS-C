/*
 * A320496  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent hexagons with respect to a point of type B.
 * https://oeis.org/A320496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-6) + a(n-16) - a(n-22) + f(n)
 * where f(n) is the coefficient of x^n in
 * f(x) = -4*x^27 + 2*x^26 - 2*x^25 + 2*x^23 + 2*x^22 + 6*x^21 + 7*x^20 +
 *        16*x^19 + 19*x^18 + 20*x^17 + 22*x^16 + 22*x^15 + 25*x^14 + 24*x^13 +
 *        25*x^12 + 26*x^11 + 21*x^10 + 25*x^9 + 25*x^8 + 23*x^7 + 21*x^6 +
 *        17*x^5 + 16*x^4 + 9*x^3 + 6*x^2 + 2*x + 1
 *
 * Initial values for n < 0 are treated as 0.  The sequence is memoised
 * in a static array so that successive calls are O(1) after the
 * necessary terms have been computed.
 */

#include <stdlib.h>
#include <stdint.h>

static const int f_coeff[28] = {
    /*  0 */ 1,
    /*  1 */ 2,
    /*  2 */ 6,
    /*  3 */ 9,
    /*  4 */ 16,
    /*  5 */ 17,
    /*  6 */ 21,
    /*  7 */ 23,
    /*  8 */ 25,
    /*  9 */ 25,
    /* 10 */ 21,
    /* 11 */ 26,
    /* 12 */ 25,
    /* 13 */ 24,
    /* 14 */ 25,
    /* 15 */ 22,
    /* 16 */ 22,
    /* 17 */ 20,
    /* 18 */ 19,
    /* 19 */ 16,
    /* 20 */ 7,
    /* 21 */ 6,
    /* 22 */ 2,
    /* 23 */ 2,
    /* 24 */ 0,
    /* 25 */ -2,
    /* 26 */ 2,
    /* 27 */ -4
};

/* static cache for memoisation */
static seq_t *cache   = NULL;
static idx_t  cache_sz = 0;

seq_t a(idx_t n)
{
    /* Grow the cache to accommodate index n */
    if (n + 1 > cache_sz) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return (seq_t)0;   /* out of memory: return 0 */
        /* Zero-initialise the new elements */
        for (idx_t i = cache_sz; i <= n; ++i) new_cache[i] = (seq_t)0;
        cache   = new_cache;
        cache_sz = n + 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        /* If the term is already computed, skip */
        if (i < cache_sz && cache[i] != (seq_t)0 && (i != 0)) continue;

        /* Compute a(i) using the recurrence */
        seq_t sum = (seq_t)0;

        /* a(i-6) */
        if (i >= 6) sum += cache[i - 6];

        /* a(i-16) */
        if (i >= 16) sum += cache[i - 16];

        /* subtract a(i-22) */
        if (i >= 22) {
            /* assume sum >= cache[i-22] for valid sequence */
            sum -= cache[i - 22];
        }

        /* add f(i) if i <= 27 */
        if (i <= 27) {
            int fi = f_coeff[i];
            if (fi >= 0)
                sum += (seq_t)fi;
            else
                sum -= (seq_t)(-fi);
        }

        cache[i] = sum;
    }

    return cache[n];
}
