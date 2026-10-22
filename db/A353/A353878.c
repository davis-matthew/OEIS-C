/*
 * A353878  Number of tilings of a 3 X n rectangle using right trominoes,
 *          dominoes and 1 X 1 tiles.
 * https://oeis.org/A353878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 33*a(n-2) + 3*a(n-3) - 40*a(n-4) + 15*a(n-5),
 *            a(0)=1, a(1)=3, a(2)=44, a(3)=369, a(4)=3633
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated size of cache      */
    static size_t  len   = 0;   /* number of filled entries     */

    /* First five terms (known from the sequence) */
    static const seq_t base[5] = {1, 3, 44, 369, 3633};

    /* Direct answer for small n */
    if (n < 5)
        return base[(size_t)n];

    /* Initialise cache with the first five terms if not already done */
    if (len < 5)
    {
        size_t need = 5;
        if (cap < need)
        {
            seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(*cache));
            cache = tmp;
            cap   = need;
        }
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        len = 5;
    }

    /* Extend cache up to index n if necessary */
    size_t target = (size_t)n + 1;      /* need elements 0..n  */
    if (len < target)
    {
        /* Grow the allocation if needed */
        if (cap < target)
        {
            size_t new_cap = (cap > 0 ? cap : 1);
            while (new_cap < target)
                new_cap = new_cap * 2 + 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute the missing terms using the recurrence */
        for (size_t i = len; i < target; ++i)
        {
            seq_t val = cache[i-1] * 6
                      + cache[i-2] * 33
                      + cache[i-3] * 3
                      - cache[i-4] * 40
                      + cache[i-5] * 15;
            cache[i] = val;
        }
        len = target;
    }

    return cache[(size_t)n];
}
