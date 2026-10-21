/*
 * A255116  Number of n-length words on {0,1,2,3} in which 0 appears only in
 *          runs of length 2.
 * https://oeis.org/A255116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-3); a(0)=1, a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;        /* cached terms */
    static size_t  cap   = 0;          /* allocated size of cache */
    static idx_t   max   = 0;          /* highest index computed so far */

    /* initialise cache once */
    if (!cache) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback on allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        for (size_t i = 3; i < cap; ++i)
            cache[i] = (seq_t)0;
        max = 2;
    }

    /* expand cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;              /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp) cache = tmp; else /* can try again with same size */ ;
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] + 3 * cache[i - 3];
        }
        max = n;
    }

    return cache[n];
}
