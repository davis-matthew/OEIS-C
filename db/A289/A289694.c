/*
 * A289694  The number of partitions of [n] with exactly 4 blocks without
 *          peaks.
 * https://oeis.org/A289694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) defined by linear recurrence a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=4, a(6)=16, a(7)=64, a(8)=236, a(9)=818,
 *            a(10)=2736, a(11)=8934, a(12)=28622, a(13)=90324, a(14)=281792,
 *            a(15)=871556, a(16)=2677750,
 *            a(n)=10*a(n-1)-45*a(n-2)+130*a(n-3)-280*a(n-4)+471*a(n-5)
 *            -643*a(n-6)+734*a(n-7)-701*a(n-8)+...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;       /* a[0] = a(1), a[1] = a(2), … */
    static idx_t max_cached = 0;      /* highest n that has been computed  */
    static size_t cap = 0;            /* allocated size of cache (elements) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;

        /* given initial values (a(1) .. a(16)) */
        cache[3]  = 1;
        cache[4]  = 4;
        cache[5]  = 16;
        cache[6]  = 64;
        cache[7]  = 236;
        cache[8]  = 818;
        cache[9]  = 2736;
        cache[10] = 8934;
        cache[11] = 28622;
        cache[12] = 90324;
        cache[13] = 281792;
        cache[14] = 871556;
        cache[15] = 2677750;
        max_cached = 16;
    }

    /* if requested value is already known, return it */
    if (n <= max_cached) return cache[n-1];

    /* grow the cache if necessary */
    if ((idx_t)cap < n) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n) newcap = n;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute values from max_cached+1 up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = 0;  /* sum of positive contributions      */
        seq_t neg = 0;  /* sum of negative contributions      */

        /* a(i) = 10*a(i-1) -45*a(i-2) +130*a(i-3) -280*a(i-4)
                +471*a(i-5) -643*a(i-6) +734*a(i-7) -701*a(i-8)
                +575*a(i-9) -400*a(i-10) +237*a(i-11) -121*a(i-12)
                +49*a(i-13) -18*a(i-14) +4*a(i-15) -a(i-16) */

        pos += mul_small(cache[i-2], 10u);      /* +10*a(i-1) */
        neg += mul_small(cache[i-3], 45u);      /* - 45*a(i-2) */
        pos += mul_small(cache[i-4], 130u);     /* +130*a(i-3) */
        neg += mul_small(cache[i-5], 280u);     /* -280*a(i-4) */
        pos += mul_small(cache[i-6], 471u);     /* +471*a(i-5) */
        neg += mul_small(cache[i-7], 643u);     /* -643*a(i-6) */
        pos += mul_small(cache[i-8], 734u);     /* +734*a(i-7) */
        neg += mul_small(cache[i-9], 701u);     /* -701*a(i-8) */
        pos += mul_small(cache[i-10], 575u);    /* +575*a(i-9) */
        neg += mul_small(cache[i-11], 400u);    /* -400*a(i-10) */
        pos += mul_small(cache[i-12], 237u);    /* +237*a(i-11) */
        neg += mul_small(cache[i-13], 121u);    /* -121*a(i-12) */
        pos += mul_small(cache[i-14], 49u);     /* +49*a(i-13) */
        neg += mul_small(cache[i-15], 18u);     /* -18*a(i-14) */
        pos += mul_small(cache[i-16], 4u);      /* +4*a(i-15) */
        neg += mul_small(cache[i-17], 1u);      /* -1*a(i-16) */

        /* final value: positive minus negative */
        seq_t val = pos - neg;
        cache[i-1] = val;
        max_cached = i;
    }

    return cache[n-1];
}
