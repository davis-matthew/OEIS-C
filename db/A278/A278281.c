/*
 * A278281  Number of 2 X n 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-a(n-3)+a(n-4) with initial values
 *            a(1)=0, a(2)=2, a(3)=4, a(4)=13
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, but safe */

    static seq_t *cache = NULL;   /* cache[0] unused, cache[i] holds a(i) */
    static size_t cap   = 0;      /* allocated size (>= max(n)+1)      */
    static size_t maxc  = 0;      /* highest n computed so far         */

    /* Ensure the cache is large enough for index n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL)            /* allocation failure – give up */
            return (seq_t)0;
        /* Initialise any new entries to zero */
        for (size_t i = cap; i < need; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = need;
    }

    /* Compute values up to n if they are missing */
    for (size_t i = maxc + 1; i <= (size_t)n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else if (i == 3)
            cache[i] = (seq_t)4;
        else if (i == 4)
            cache[i] = (seq_t)13;
        else
            cache[i] = cache[i-1] * (seq_t)2
                       + cache[i-2] * (seq_t)3
                       - cache[i-3]
                       + cache[i-4];
    }
    maxc = (size_t)n;
    return cache[n];
}
