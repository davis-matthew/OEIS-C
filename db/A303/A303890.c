/*
 * A303890  Number of nX2 0..1 arrays with every element unequal to 1, 2, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 4*a(n-3) + 4*a(n-4) with initial
 *            values a(1)=1, a(2)=7, a(3)=23, a(4)=86
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements seq_t a(idx_t n) computing the OEIS sequence A303890 */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of already computed terms   */
    static idx_t  size  = 0;      /* current allocated size (indices 0..size-1) */
    static idx_t  last  = 0;      /* largest index computed so far      */

    if (n == 0)          /* safety: OEIS offset starts at 1              */
        return 0;

    /* Resize the cache so that it can hold index n */
    if (!cache || n >= size) {
        idx_t newsize = n + 1; /* indices 0..n inclusive                           */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();    /* unlikely to happen, but safe nonetheless   */
        cache = tmp;
        size  = newsize;
    }

    /* Compute missing terms up to n, if necessary */
    if (n > last) {
        /* Initial values for indices 1..4 */
        static const seq_t init[5] = { 0, 1, 7, 23, 86 };
        idx_t i;

        /* Fill in initial values that have not yet been set */
        for (i = last + 1; i <= n && i <= 4; ++i)
            cache[i] = init[i];

        /* Start recurrence from the first index that requires it */
        idx_t start = (last >= 4) ? last + 1 : 5;
        if (start < 5) start = 5; /* ensure start is at least 5 */

        for (i = start; i <= n; ++i) {
            cache[i] = 3 * cache[i-1]
                     +   cache[i-2]
                     + 4 * cache[i-3]
                     + 4 * cache[i-4];
        }

        last = n;
    }

    return cache[n];
}
