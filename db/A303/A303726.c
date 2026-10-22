/*
 * A303726  Number of nX7 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 35, u64 n <= 78, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A303726 – Number of n×7 0/1 arrays with upper‑left zero,
    arrays in which no 0 or 1 is adjacent (by king's move) to a 0,1,2 or 3.
    Empirical linear recurrence (order 10):
        a(n) = 2*a(n-1) - a(n-4) - a(n-5) + a(n-7)
               - a(n-8) - a(n-9) + 2*a(n-10)   for n > 18
    Initial values (n = 1 … 18):
        64, 719, 294, 406, 564, 894, 1476, 2252, 3461, 5631,
        9362, 15569, 26111, 43960, 73883, 124371, 209956, 354631
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache : a[0] unused, a[1] = first term */
    static seq_t *cache   = NULL;   /* dynamically allocated array */
    static size_t  cap    = 0;      /* current capacity of cache */
    static size_t  computed = 0;    /* highest n computed so far */

    /* Initial values for n = 1 … 18 */
    static const seq_t init[19] = {
        0,          /* a(0) unused */
        64, 719, 294, 406, 564, 894, 1476, 2252, 3461, 5631,
        9362, 15569, 26111, 43960, 73883, 124371, 209956, 354631
    };

    /* Ensure cache big enough for all indices up to n */
    if (cache == NULL) {
        /* start with capacity for the initial 18 terms */
        cap = 19;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
    } else if (n + 1 > cap) {
        size_t newcap = (n + 1 > cap * 2 ? n + 1 : cap * 2);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* Compute terms beyond what we already have */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i <= 18)
        {
            cache[i] = init[i];
        }
        else
        {
            /* 2*a(i-1) - a(i-4) - a(i-5) + a(i-7) - a(i-8) - a(i-9) + 2*a(i-10) */
            /* compute in an order that avoids underflow in unsigned arithmetic   */
            seq_t val = cache[i-1] + cache[i-1];           /* 2*a(i-1)          */
            val += cache[i-7];                             /* + a(i-7)          */
            val += cache[i-10] + cache[i-10];              /* + 2*a(i-10)       */
            seq_t sub = cache[i-4] + cache[i-5] +
                        cache[i-8] + cache[i-9];           /* a(i-4)+a(i-5)+a(i-8)+a(i-9) */
            val -= sub;                                   /* subtract the sum   */
            cache[i] = val;
        }
    }

    computed = n;
    return cache[n];
}
