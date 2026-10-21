/*
 * A286390  a(n) = a(n-2) - 2*a(n-3) + a(n-4) for n>3, a(0)=0, a(1)=2,
 *          a(2)=-1, a(3)=3.
 * https://oeis.org/A286390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 45, i64 n <= 91, i128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) - 2*a(n-3) + a(n-4)  (n > 3)
   Initial values: a(0)=0, a(1)=2, a(2)=-1, a(3)=3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise base terms once */
    if (!cache)
    {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)3;
    }

    /* grow cache if needed */
    if (n + 1 > cache_size)
    {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

        /* compute missing terms using the recurrence */
        for (idx_t i = old_size; i < cache_size; ++i)
        {
            cache[i] = cache[i - 2] - (seq_t)2 * cache[i - 3] + cache[i - 4];
        }
    }

    return cache[n];
}
