/*
 * A387871  a(n) = Sum_{k=0..n} binomial(2*n+1,5*k).
 * https://oeis.org/A387871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+1, 5*k) Recurrence: a(n) =
 *            5*a(n-1) - 10*a(n-2) + 20*a(n-3) + 15*a(n-4) + 4*a(n-5) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=22, a(4)=127
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..len-1] already computed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* extend cache up to n if necessary */
    if (n >= cache_len)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache = new_cache;

        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            if (i == 0)          cache[0] = 1;
            else if (i == 1)      cache[1] = 1;
            else if (i == 2)      cache[2] = 2;
            else if (i == 3)      cache[3] = 22;
            else if (i == 4)      cache[4] = 127;
            else
            {
                seq_t t = 5 * cache[i - 1];
                t -= 10 * cache[i - 2];
                t += 20 * cache[i - 3];
                t += 15 * cache[i - 4];
                t += 4  * cache[i - 5];
                cache[i] = t;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
