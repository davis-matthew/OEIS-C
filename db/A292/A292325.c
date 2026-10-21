/*
 * A292325  p-INVERT of (1,0,0,0,1,0,0,0,0,0,...), where p(S) = (1 - S)^2.
 * https://oeis.org/A292325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-5) - 2*a(n-6) - a(n-10) (for n
 *            ≥ 11) Initial values: a(0)=2, a(1)=3, a(2)=4, a(3)=5, a(4)=8,
 *            a(5)=13, a(6)=20, a(7)=29, a(8)=40, a(9)=56, a(10)=80
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 67, u64 n <= 143,
 *            u128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;     /* pointer to cache array      */
    static idx_t cap   = 0;         /* allocated capacity         */
    static idx_t last  = 0;         /* largest index computed     */

    /* ensure the cache can hold indices up to n */
    if (n >= cap) {
        idx_t newcap = (cap == 0) ? 11 : cap * 2;
        if (newcap <= n) newcap = n + 1;          /* at least n+1 */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {                       /* out of memory: abort */
            exit(1);
        }
        cache = tmp;
        cap   = newcap;

        /* initialise the first 11 terms once */
        if (cap >= 11 && last < 10) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)3;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)5;
            cache[4] = (seq_t)8;
            cache[5] = (seq_t)13;
            cache[6] = (seq_t)20;
            cache[7] = (seq_t)29;
            cache[8] = (seq_t)40;
            cache[9] = (seq_t)56;
            cache[10] = (seq_t)80;
            last = 10;
        }
    }

    /* compute missing terms up to n (if any) */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 10) {
            /* already initialised */
            continue;
        }
        cache[i] = (seq_t)2 * cache[i - 1]                /* 2*a(i-1) */
                 - cache[i - 2]                             /* - a(i-2) */
                 + (seq_t)2 * cache[i - 5]                  /* + 2*a(i-5) */
                 - (seq_t)2 * cache[i - 6]                  /* - 2*a(i-6) */
                 - cache[i - 10];                           /* - a(i-10) */
    }
    if (n > last) last = n;

    return cache[n];
}
