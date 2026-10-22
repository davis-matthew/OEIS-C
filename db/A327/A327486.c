/*
 * A327486  Product of Omegas of positive integers from 2 to n.
 * https://oeis.org/A327486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * A001222(n) for n > 1; a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] = a(i) */
    static idx_t cache_size = 0;  /* maximum index currently stored */

    /* Handle n <= 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Initial allocation */
    if (!cache)
    {
        cache_size = 1;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;   /* a(1) = 1 */
    }

    /* Extend cache if necessary */
    if (n > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t omega = bigomega(i);          /* Ω(i) */
            cache[i] = cache[i - 1] * omega;    /* cumulative product */
        }

        cache_size = n;
    }

    return cache[n];
}
