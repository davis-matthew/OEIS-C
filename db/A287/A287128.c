/*
 * A287128  a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5), where
 *          a(0) = 2, a(1) =3, a(2) = 6, a(3)=13, a(4) = 29.
 * https://oeis.org/A287128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5)
   a(0)=2, a(1)=3, a(2)=6, a(3)=13, a(4)=29 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size > n)
        return cache[n];

    /* allocate or grow the cache to hold indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);                /* malloc failed */
    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = n + 1;

    /* initialise base terms up to index 4 (or n if smaller) */
    idx_t init_max = n < 5 ? n : 4;
    for (idx_t i = 0; i <= init_max; ++i)
    {
        switch (i)
        {
            case 0: cache[i] = (seq_t)2; break;
            case 1: cache[i] = (seq_t)3; break;
            case 2: cache[i] = (seq_t)6; break;
            case 3: cache[i] = (seq_t)13; break;
            case 4: cache[i] = (seq_t)29; break;
        }
    }

    /* compute missing terms from max(old_size,5) up to n */
    idx_t start = old_size > 5 ? old_size : 5;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t val = 3 * cache[i - 1];
        val -= 2 * cache[i - 2];
        val += 2 * cache[i - 3];
        val -= 4 * cache[i - 4];
        val += 2 * cache[i - 5];
        cache[i] = val;
    }

    return cache[n];
}
