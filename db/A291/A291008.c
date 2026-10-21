/*
 * A291008  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 7*S^2.
 * https://oeis.org/A291008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291008
    Formula: a(0)=0, a(1)=7, a(2)=14
             a(n)=2*a(n-1)+6*a(n-2)   for n≥3
    (recurrence derived from 7*x/(1-2x-6x²)) */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation array          */
    static idx_t  cap   = 0;      /* current allocated capacity */
    static idx_t  len   = 0;      /* number of initialised terms*/

    /* Grow the cache if the requested index is out of bounds. */
    if ((idx_t)n + 1 > cap) {
        idx_t newcap = (cap ? cap : 1);
        while (newcap <= n) newcap <<= 1;           /* double size */
        cache = (seq_t*)realloc(cache, (size_t)newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Initialise the first three known values once. */
    if (len == 0) {
        cache[0] = (seq_t)0;
        len = 1;
    }
    if (len <= 1 && n >= 1) {
        cache[1] = (seq_t)7;
        len = 2;
    }
    if (len <= 2 && n >= 2) {
        cache[2] = (seq_t)14;
        len = 3;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = ((seq_t)2) * cache[i-1] + ((seq_t)6) * cache[i-2];
    }
    len = n + 1;   /* we now have all values up to n */

    return cache[n];
}
