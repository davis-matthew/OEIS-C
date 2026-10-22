/*
 * A317148  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition (order‑16) for the OEIS A317148 sequence:
 * a(1) = 4, a(2) = 14, …, a(16) = 220178291.
 * For n > 16:
 *   a(n) = 2*a(n-1) + 5*a(n-2) +10*a(n-3)
 *          -22*a(n-4) -56*a(n-5) -48*a(n-6)
 *          +100*a(n-7) +180*a(n-8) +100*a(n-9)
 *          -158*a(n-10) -172*a(n-11) -21*a(n-12)
 *          +38*a(n-13) -33*a(n-14) -36*a(n-15).
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* 1‑based cache: cache[i] = a(i)  */
static idx_t  cache_max = 0;  /* highest index currently cached   */

/* Initial 16 terms */
static const seq_t init[17] = {
    0,          /* dummy, unused index 0 */
    4,          /* a(1) */
    14,         /* a(2) */
    36,         /* a(3) */
    102,        /* a(4) */
    389,        /* a(5) */
    1251,       /* a(6) */
    4091,       /* a(7) */
    13931,      /* a(8) */
    46536,      /* a(9) */
    155280,     /* a(10) */
    521208,     /* a(11) */
    1746936,    /* a(12) */
    5849787,    /* a(13) */
    19606077,   /* a(14) */
    65709643,   /* a(15) */
    220178291   /* a(16) */
};

seq_t a(idx_t n)
{
    /* Guard against calls with n = 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* Allocate or enlarge the cache to hold up to index n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        cache_max = 0;
    } else if (n > cache_max) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i] = init[(size_t)i];
        } else {
            /* Positive contributions */
            seq_t pos = 2 * cache[i - 1]
                      + 5 * cache[i - 2]
                      +10 * cache[i - 3]
                      +100 * cache[i - 7]
                      +180 * cache[i - 8]
                      +100 * cache[i - 9]
                      + 38 * cache[i -13];

            /* Negative contributions */
            seq_t neg = 22 * cache[i - 4]
                      + 56 * cache[i - 5]
                      + 48 * cache[i - 6]
                      +158 * cache[i -10]
                      +172 * cache[i -11]
                      + 21 * cache[i -12]
                      + 33 * cache[i -14]
                      + 36 * cache[i -15];

            /* Since the recurrence guarantees pos ≥ neg for all i>16,
             * the subtraction is safe even with unsigned arithmetic. */
            cache[i] = pos - neg;
        }
    }
    cache_max = n;
    return cache[n];
}
