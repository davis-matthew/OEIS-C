/*
 * A264528  Number of (n+1)X(2+1) arrays of permutations of 0..n*3+2 with each
 *          element having directed index change -2,-2 -1,0 0,1 or 1,0.
 * https://oeis.org/A264528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = 10*a(n-2) + 6*a(n-3) - 42*a(n-4) - 40*a(n-5)
 *            + 117*a(n-6) + 143*a(n-7) - 222*a(n-8) - 266*a(n-9)
 *            + 272*a(n-10) + 266*a(n-11) - 222*a(n-12) - 143*a(n-13)
 *            + 117*a(n-14) + 40*a(n-15) - 42*a(n-16) - 6*a(n-17)
 *            + 10*a(n-18) - a(n-20)
 * Initial values: a(1)=1, a(2)=4, a(3)=4, a(4)=19, a(5)=49, a(6)=100,
 *                 a(7)=316, a(8)=766, a(9)=1883, a(10)=5221,
 *                 a(11)=12797, a(12)=33376, a(13)=86922, a(14)=220743,
 *                 a(15)=573264, a(16)=1477256, a(17)=3792956,
 *                 a(18)=9804491, a(19)=25237000, a(20)=64989265.
 *
 * The function works for any n >= 1.  It memoises results in a static
 * dynamic array so subsequent calls (in any order) are O(1).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* undefined */

    /* Memoisation cache */
    static seq_t *cache = NULL;                           /* allocated array */
    static size_t cache_cap = 0;                          /* current capacity (max index) */
    static idx_t  cache_len = 0;                          /* next index to compute */

    /* Initialise cache with the first 20 terms on the first call */
    if (cache == NULL) {
        cache_cap = 64;                                   /* initial capacity > 20 */
        cache     = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        /* If allocation fails in an absurd situation just return 0 */
        if (!cache) return 0;

        /* First 20 terms of the sequence */
        static const seq_t init[21] = {
            0,              /* a(0) unused */
            1,              /* a(1) */
            4,              /* a(2) */
            4,              /* a(3) */
            19,             /* a(4) */
            49,             /* a(5) */
            100,            /* a(6) */
            316,            /* a(7) */
            766,            /* a(8) */
            1883,           /* a(9) */
            5221,           /* a(10) */
            12797,          /* a(11) */
            33376,          /* a(12) */
            86922,          /* a(13) */
            220743,         /* a(14) */
            573264,         /* a(15) */
            1477256,        /* a(16) */
            3792956,        /* a(17) */
            9804491,        /* a(18) */
            25237000,       /* a(19) */
            64989265        /* a(20) */
        };
        for (idx_t i = 0; i <= 20; ++i) cache[i] = init[i];
        cache_len = 21;                                   /* next index to compute */
    }

    /* If the requested value is already cached, return it */
    if (n < cache_len) return cache[n];

    /* Grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;                       /* grow to n */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                               /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from the current cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* Positive coefficient terms */
        pos += 10 * cache[i - 2];
        pos +=  6 * cache[i - 3];
        pos += 117 * cache[i - 6];
        pos += 143 * cache[i - 7];
        pos += 272 * cache[i -10];
        pos += 266 * cache[i -11];
        pos += 117 * cache[i -14];
        pos +=  40 * cache[i -15];
        pos += 10 * cache[i -18];

        /* Negative coefficient terms */
        neg += 42 * cache[i - 4];
        neg += 40 * cache[i - 5];
        neg += 222 * cache[i - 8];
        neg += 266 * cache[i - 9];
        neg += 222 * cache[i -12];
        neg += 143 * cache[i -13];
        neg +=  42 * cache[i -16];
        neg +=   6 * cache[i -17];
        neg +=     cache[i -20];                          /* coefficient 1 */

        cache[i] = pos - neg;
    }

    cache_len = n + 1;
    return cache[n];
}
