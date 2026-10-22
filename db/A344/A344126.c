/*
 * A344126  Coordination sequence for the hypertriangular lattice.
 * https://oeis.org/A344126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A344126: Coordination sequence for the hypertriangular lattice.
   The generating function is
       (1+5*x+18*x^2+22*x^3+28*x^4+16*x^5+7*x^6-3*x^7+2*x^8) /
       ((x^2+x+1)^2 * (1-x)^3).
   From this one obtains the linear recurrence
       a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7)   (n ≥ 9)
   with the base values
       a(0)=1, a(1)=6, a(2)=24, a(3)=48, a(4)=86,
       a(5)=138, a(6)=192, a(7)=260, a(8)=348.
   The function below memoises results in a static cache so that any
   ordering of calls works efficiently.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  capacity = 0;   /* allocated size of cache array   */
    static idx_t  last     = 0;   /* highest index that has been filled */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 16;                 /* initial capacity, more than base size */
        cache    = (seq_t *)malloc(sizeof(seq_t) * capacity);
        /* base values, a[0]..a[8] */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)48;
        cache[4] = (seq_t)86;
        cache[5] = (seq_t)138;
        cache[6] = (seq_t)192;
        cache[7] = (seq_t)260;
        cache[8] = (seq_t)348;
        last     = 8;
    }

    /* if requested index already computed, return it directly */
    if (n <= last)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity : 16;
        while (new_cap <= n)  /* new_cap > n after loop */
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* compute missing terms sequentially from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence:
              a(i) = a(i-1) + 2*a(i-3) - 2*a(i-4) - a(i-6) + a(i-7)
        */
        seq_t val = cache[i-1]          /* a(i-1) */
                 + 2 * cache[i-3]      /* +2*a(i-3) */
                 + cache[i-7];         /* + a(i-7)  */
        val -= 2 * cache[i-4];          /* -2*a(i-4) */
        val -= cache[i-6];              /* -a(i-6)   */
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
