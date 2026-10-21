/*
 * A247215  Integers k such that 3k+1 and 6k+1 are both squares.
 * https://oeis.org/A247215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=8, a(3)=280,
   a(n)=35*a(n-1)-35*a(n-2)+a(n-3) for n>=4 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Base cases for the first three terms */
    static const seq_t base[3] = { (seq_t)0, (seq_t)8, (seq_t)280 };

    /* If we are only asked for one of the first three, return immediately */
    if (n <= 3)
        return base[n - 1];

    /* Lazily allocate and initialise a dynamic cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* number of allocated slots */
    static idx_t   sz    = 0;   /* number of elements actually stored */

    if (cache == NULL) {
        /* Ensure we have at least 3 slots for the base values */
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)   /* allocation failure – fallback to base */
            return base[2];
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = base[i];
        sz = 3;
    }

    /* Resize if necessary for the requested index */
    if (n > (idx_t)cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure – give up gracefully */
            return base[2];
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = sz; i < n; ++i) {
        /* a(i+1) = 35*a(i) - 35*a(i-1) + a(i-2) */
        cache[i] = (seq_t)35 * cache[i - 1]
                 - (seq_t)35 * cache[i - 2]
                 + cache[i - 3];
    }
    sz = n;

    return cache[n - 1];
}
