/*
 * A251212  Number of (n+1) X (1+1) 0..1 arrays with no 2 X 2 subblock having
 *          zero or two 1's.
 * https://oeis.org/A251212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3), with a(1) = 9, a(2) = 21,
 *            a(3) = 49
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For any other value,
       return 0. It may be called in any order. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache: cache[i] holds a(i) for i >= 1.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* maximum index currently stored */

    size_t idx = (size_t)n;          /* n is guaranteed to fit in size_t */

    /* Expand cache if needed */
    if (idx > cache_size)
    {
        size_t new_size = idx + 1;   /* allocate indices 0 .. idx */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;        /* allocation failed – return 0 */

        cache = new_cache;

        /* Initialize new entries */
        for (size_t i = cache_size + 1; i <= idx; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)9;
            else if (i == 2)
                cache[i] = (seq_t)21;
            else if (i == 3)
                cache[i] = (seq_t)49;
            else
            {
                /* recurrence: a(i) = a(i-1) + 4*a(i-2) - 2*a(i-3) */
                seq_t v1 = cache[i - 1];
                seq_t v2 = cache[i - 2] * (seq_t)4;
                seq_t v3 = cache[i - 3] * (seq_t)2;
                cache[i] = v1 + v2 - v3;
            }
        }
        cache_size = idx;
    }

    return cache[idx];
}
