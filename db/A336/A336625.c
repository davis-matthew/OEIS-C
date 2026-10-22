/*
 * A336625  Indices of triangular numbers that are eight times other
 *          triangular numbers.
 * https://oeis.org/A336625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 34*a(n-2) - a(n-4) + 16 for n ≥ 5 with initial values
 *            a(1)=0, a(2)=15, a(3)=32, a(4)=527
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* pointer to allocated array */
    static idx_t cache_size = 0;  /* maximum index currently stored */

    if (n == 0)                     /* the sequence is defined for n ≥ 1 */
        return 0;

    /* Ensure the cache has room for indices up to n */
    if (n > cache_size)
    {
        /* new buffer size (indices 0..n, but we ignore index 0) */
        size_t new_size = (size_t)n + 1;

        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* initialise new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            if (i == 1)
                cache[1] = (seq_t)0;
            else if (i == 2)
                cache[2] = (seq_t)15;
            else if (i == 3)
                cache[3] = (seq_t)32;
            else if (i == 4)
                cache[4] = (seq_t)527;
            else
            {
                /* i ≥ 5 : recurrence requires indices ≥ 1 */
                cache[i] = cache[i - 2] * (seq_t)34 - cache[i - 4] + (seq_t)16;
            }
        }
        cache_size = n;   /* update maximum index stored */
    }

    return cache[n];
}
