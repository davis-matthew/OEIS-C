/*
 * A313981  Coordination sequence Gal.5.319.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A313981
   Recurrence: a(n) = -a(n-1) - a(n-2) + a(n-5) + a(n-6) + 2*a(n-7)
              + a(n-8) + a(n-9) - a(n-12) - a(n-13) - a(n-14)
   Initial values (n = 0 … 14):
   1, 5, 10, 17, 22, 29, 34, 37, 46, 49, 54, 63, 66, 73, 78
*/

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 15 initial terms, static so they are constants */
    static const seq_t init[15] = {
        1, 5, 10, 17, 22, 29, 34, 37, 46, 49, 54, 63, 66, 73, 78
    };

    /* dynamic cache of computed values */
    static seq_t *cache  = NULL; /* points to array of seq_t */
    static idx_t  len    = 0;    /* number of elements stored (next free index) */
    static idx_t  cap    = 0;    /* allocated capacity */

    /* first call: allocate and initialise cache with the 15 known terms */
    if (cache == NULL)
    {
        /* ensure capacity at least 15 */
        cap = 15;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        len = 15; /* elements 0 … 14 are ready */
    }

    /* if requested index already in cache, return it */
    if (n < len)
        return cache[n];

    /* increase capacity if needed */
    if (n >= cap)
    {
        size_t newcap = cap;
        /* grow until newcap > n */
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* compute terms from len up to n inclusive */
    for (idx_t i = len; i <= n; ++i)
    {
        /* apply the recurrence; all indices are valid since i >= 15 */
        seq_t tmp = cache[i - 5]            /* a(i-5)   */
                 + cache[i - 6]            /* a(i-6)   */
                 + 2 * cache[i - 7]       /* 2*a(i-7) */
                 + cache[i - 8]            /* a(i-8)   */
                 + cache[i - 9]            /* a(i-9)   */

                 - cache[i - 1]            /* -a(i-1) */
                 - cache[i - 2]            /* -a(i-2) */
                 - cache[i -12]           /* -a(i-12)*/
                 - cache[i -13]           /* -a(i-13)*/
                 - cache[i -14];          /* -a(i-14)*/

        cache[i] = tmp;
    }
    len = n + 1; /* now cache holds indices 0 … n */
    return cache[n];
}
