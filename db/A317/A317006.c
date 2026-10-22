/*
 * A317006  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 5*a(n-1) -7*a(n-3) -27*a(n-4) -5*a(n-5) +14*a(n-6)+78*a(n-7)
 *              +49*a(n-8)-46*a(n-9)-81*a(n-10)+5*a(n-11)+55*a(n-12)
 *              -2*a(n-13)-39*a(n-14)-30*a(n-15)-8*a(n-16)   for n>17
 * Initial values:
 *   a(1)=4,  a(2)=23,  a(3)=96,  a(4)=400,  a(5)=1704,  a(6)=7313,
 *   a(7)=31328, a(8)=134118, a(9)=574442, a(10)=2460748,
 *   a(11)=10541124, a(12)=45155605, a(13)=193438738,
 *   a(14)=828663631, a(15)=3549885894,
 *   a(16)=15207276051, a(17)=65146190696
 *
 * The function memoises previously computed values.
 */

#include <stdlib.h>

/* 18 elements: index 0 unused, 1..17 initial terms */
static const seq_t _init_vals[18] = {
    (seq_t)0,                      /* unused 0‑th index */
    (seq_t)4,                      /* a(1)   */
    (seq_t)23,                     /* a(2)   */
    (seq_t)96,                     /* a(3)   */
    (seq_t)400,                    /* a(4)   */
    (seq_t)1704,                   /* a(5)   */
    (seq_t)7313,                   /* a(6)   */
    (seq_t)31328,                  /* a(7)   */
    (seq_t)134118,                 /* a(8)   */
    (seq_t)574442,                 /* a(9)   */
    (seq_t)2460748,                /* a(10)  */
    (seq_t)10541124,               /* a(11)  */
    (seq_t)45155605,               /* a(12)  */
    (seq_t)193438738,              /* a(13)  */
    (seq_t)828663631,              /* a(14)  */
    (seq_t)3549885894,             /* a(15)  */
    (seq_t)15207276051,            /* a(16)  */
    (seq_t)65146190696             /* a(17)  */
};

seq_t a(idx_t n)
{
    /* indices below 1 are not part of OEIS, return 0 */
    if (n < 1) return (seq_t)0;

    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* one past highest allocated index */
    static size_t last_computed = 0;/* highest index already computed */

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = ((size_t)n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* should never happen for valid inputs */
        /* initialise new elements to 0 */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values up to n */
    for (size_t k = last_computed + 1; k <= (size_t)n; ++k) {
        seq_t val;
        if (k <= 17) {
            /* use pre‑computed values */
            val = _init_vals[(idx_t)k];
        } else {
            /* recurrence for k>17 */
            /* compute positive and negative sums separately to avoid
             * intermediate underflow in unsigned arithmetic.  The
             * recurrence guarantees that the positive sum is ≥ the negative sum. */
            seq_t pos = (seq_t)5   * cache[k - 1]  /* a(n-1)   */
                      + (seq_t)14  * cache[k - 6]  /* a(n-6)   */
                      + (seq_t)78  * cache[k - 7]  /* a(n-7)   */
                      + (seq_t)49  * cache[k - 8]  /* a(n-8)   */
                      + (seq_t)5   * cache[k - 11] /* a(n-11)  */
                      + (seq_t)55  * cache[k - 12];/* a(n-12)  */

            seq_t neg = (seq_t)7   * cache[k - 3]  /* a(n-3)   */
                      + (seq_t)27  * cache[k - 4]  /* a(n-4)   */
                      + (seq_t)5   * cache[k - 5]  /* a(n-5)   */
                      + (seq_t)46  * cache[k - 9]  /* a(n-9)   */
                      + (seq_t)81  * cache[k - 10] /* a(n-10)  */
                      + (seq_t)2   * cache[k - 13] /* a(n-13)  */
                      + (seq_t)39  * cache[k - 14] /* a(n-14)  */
                      + (seq_t)30  * cache[k - 15] /* a(n-15)  */
                      + (seq_t)8   * cache[k - 16];/* a(n-16)  */

            val = pos - neg;
        }
        cache[(size_t)k] = val;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
