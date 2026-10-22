/*
 * A341270  a(n) = Sum_{k=1..n} a(n mod k) for n > 0; a(0) = 1.
 * https://oeis.org/A341270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} a(n mod k), with a(0) = 1
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sequence A341270 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend cache if necessary */
    if (n + 1 > cache_size)
    {
        /* resize the cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL)
            return (seq_t)0; /* allocation failed – return zero */
        cache = tmp;

        /* compute missing entries */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i % k];
            cache[i] = sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
