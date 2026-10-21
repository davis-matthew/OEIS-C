/*
 * A249665  The number of permutations p of {1,...,n} such that p(1)=1,
 *          p(n)=n, and |p(i)-p(i+1)| is in {1,2,3} for all i from 1 to n-1.
 * https://oeis.org/A249665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + a(n-4) + a(n-5) - a(n-7) -
 *            a(n-8) with initial values: a(1)=1, a(2)=1, a(3)=1, a(4)=2,
 *            a(5)=6, a(6)=14, a(7)=28, a(8)=56. The sequence is defined for n
 *            ≥ 1
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity (>= n+1) */
    static idx_t   last  = 0;   /* highest index already computed */

    /* Ensure cache is large enough for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 16;            /* spare room to avoid many reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                        /* fatal, should not happen */
        /* Initialise the newly allocated part to zero */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base values once */
    if (last == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)6;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)28;
        cache[8] = (seq_t)56;
        last   = 8;
    }

    /* Compute up to n if needed (calls may be out of order) */
    while (last < (idx_t)n) {
        ++last;  /* compute cache[last] */

        /* Use the recurrence for last ≥ 9 */
        if (last >= 9) {
            cache[last] =
                2 * cache[last-1]            /* 2*a(n-1) */
              - cache[last-2]                /* -a(n-2)  */
              + 2 * cache[last-3]            /* +2*a(n-3)*/
              + cache[last-4]                /* +a(n-4)  */
              + cache[last-5]                /* +a(n-5)  */
              - cache[last-7]                /* -a(n-7)  */
              - cache[last-8];               /* -a(n-8)  */
        }
        /* For last < 9 the values were already set. */
    }

    return cache[n];
}
