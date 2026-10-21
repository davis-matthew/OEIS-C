/*
 * A259635  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 or 00000001.
 * https://oeis.org/A259635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + a(n-4) for n>5; a(1)=18, a(2)=29,
 *            a(3)=69, a(4)=133, a(5)=236
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 63, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (indices 0..cap-1) */
    static size_t last  = 0;   /* highest index computed so far */

    /* adjust allocation if necessary */
    if (cap <= (size_t)n)
    {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) /* allocation failed – abort */
            abort();
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms */
    if (n > last)
    {
        /* fixed initial values for 1..5 */
        static const seq_t init[6] = {0, 18, 29, 69, 133, 236};

        for (size_t i = last + 1; i <= (size_t)n; ++i)
        {
            if (i <= 5)
                cache[i] = init[i];
            else
                cache[i] = cache[i-1] + (cache[i-3] * 3) + cache[i-4];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
