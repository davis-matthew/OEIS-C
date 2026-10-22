/*
 * A301773  Number of odd chordless cycles in the 2n-Moebius ladder graph.
 * https://oeis.org/A301773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+a(n-2)+2*a(n-3)-a(n-4)
 * Initial values: a(0)=0, a(1)=4, a(2)=8, a(3)=16
 */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache: values that have already been computed */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* If requested index is beyond current cache, extend it */
    if ((size_t)n >= cap)
    {
        size_t newcap = (size_t)n + 1;
        if (newcap < 4)
            newcap = 4;                      /* need room for first 4 terms */

        /* Reallocate the cache */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();                          /* allocation failure */

        cache = tmp;

        /* If this is the first time we allocate, initialise first terms */
        if (cap == 0)
        {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)8;
            cache[3] = (seq_t)16;
            cap = 4;
        }

        /* Compute any new terms requested */
        for (size_t i = cap; i <= (size_t)n; ++i)
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = a1 * (seq_t)2 + a2 + a3 * (seq_t)2 - a4;
        }
        cap = newcap;
    }

    return cache[n];
}
