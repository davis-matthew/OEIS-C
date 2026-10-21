/*
 * A267184  Row 4 of the square array in A267181.
 * https://oeis.org/A267184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5),
 *            a(0)=1,a(1)=4,a(2)=2,a(3)=5,a(4)=0,a(5)=5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;
    static size_t cache_cap = 0;

    /* Initialise cache with known initial values */
    if (!cache)
    {
        /* Ensure room for at least the first six terms */
        cache_cap = 6 > (size_t)n + 1 ? 6 : (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely, but guard */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)5;
        cache_last = 5;
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_last)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* unlikely, but guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
    }
    cache_last = n;
    return cache[n];
}
