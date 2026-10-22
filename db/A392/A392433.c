/*
 * A392433  a(n) = Sum_{k=0..floor(n/2)} binomial(3*k,2*n-4*k).
 * https://oeis.org/A392433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(3*k, 2*n-4*k)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Implemented via the recurrence: a(n)=2*a(n-2)+6*a(n-3)-a(n-4)+3*a(n-5)-3*a(n-6)+a(n-7) */
/* Base values: a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=1, a(5)=15, a(6)=16 */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* allocate or grow cache to hold indices 0..n */
    size_t need = (size_t)n + 1;
    if (need > cache_size)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failed */

        /* compute new values */
        for (size_t i = cache_size; i < need; ++i)
        {
            if (i == 0)         new_cache[i] = (seq_t)1;
            else if (i == 1)    new_cache[i] = (seq_t)0;
            else if (i == 2)    new_cache[i] = (seq_t)1;
            else if (i == 3)    new_cache[i] = (seq_t)3;
            else if (i == 4)    new_cache[i] = (seq_t)1;
            else if (i == 5)    new_cache[i] = (seq_t)15;
            else if (i == 6)    new_cache[i] = (seq_t)16;
            else
            {
                seq_t val = 0;
                val += 2 * new_cache[i - 2];
                val += 6 * new_cache[i - 3];
                val -= new_cache[i - 4];
                val += 3 * new_cache[i - 5];
                val -= 3 * new_cache[i - 6];
                val += new_cache[i - 7];
                new_cache[i] = val;
            }
        }

        cache = new_cache;
        cache_size = need;
    }

    return cache[(size_t)n];
}
