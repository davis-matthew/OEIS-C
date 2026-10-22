/*
 * A356411  Sum of powers of roots of x^3 - x^2 - x - 3.
 * https://oeis.org/A356411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3); a(0)=3, a(1)=1, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* array of computed terms */
    static idx_t  size = 0;      /* number of terms initialized */
    static idx_t  cap  = 0;      /* current allocation size */

    /* initialise the cache on the first call */
    if (size == 0) {
        cap = 8;                      /* initial allocation */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0;   /* allocation failure (should not happen) */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        size = 3;
    }

    /* grow the cache if the requested index is beyond the allocated space */
    if (n >= (idx_t)cap) {
        idx_t newcap = (n < 8) ? 8 : n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;   /* allocation failure (should not happen) */
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to n */
    while (size <= n) {
        cache[size] = cache[size-1] + cache[size-2] + (seq_t)3 * cache[size-3];
        ++size;
    }

    return cache[(idx_t)n];
}
