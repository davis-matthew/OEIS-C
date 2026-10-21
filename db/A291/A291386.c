/*
 * A291386  a(n) = (1/3)*A099432(n+1).
 * https://oeis.org/A291386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 18*a(n-3) - 9*a(n-4) for n>=5
 *            initial values: a(0)=2, a(1)=11, a(2)=54, a(3)=252, a(4)=1134
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* extend the cache to include index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;          /* allocation failure: return 0 */
    cache = tmp;
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0)            cache[i] = (seq_t)2;
        else if (i == 1)       cache[i] = (seq_t)11;
        else if (i == 2)       cache[i] = (seq_t)54;
        else if (i == 3)       cache[i] = (seq_t)252;
        else if (i == 4)       cache[i] = (seq_t)1134;
        else
        {
            seq_t t = cache[i-1] * (seq_t)6;
            t -= cache[i-2] * (seq_t)3;
            t -= cache[i-3] * (seq_t)18;
            t -= cache[i-4] * (seq_t)9;
            cache[i] = t;
        }
    }
    cache_size = new_size;
    return cache[(size_t)n];
}
