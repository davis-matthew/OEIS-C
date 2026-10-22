/*
 * A338154  a(n) is the number of acyclic orientations of the edges of the
 *          n-antiprism.
 * https://oeis.org/A338154
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2^(1−n)*((7−sqrt(13))^n + (7+sqrt(13))^n) + 9^n + 5
 *            Recurrence: a(n) = 17*a(n-1) − 88*a(n-2) + 153*a(n-3) −
 *            81*a(n-4) for n>6 Initial values (offset 3): a(3)=426,
 *            a(4)=4968, a(5)=50640, a(6)=486930
 * Range:     u8 none, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n < 3, as the offset is 3 */
    if (n < 3)
        return 0;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Initialize cache with the first four terms */
    if (cache_size == 0)
    {
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* a(3) … a(6) */
        cache[0] = 426;          /* n = 3 */
        cache[1] = 4968;         /* n = 4 */
        cache[2] = 50640;        /* n = 5 */
        cache[3] = 486930;       /* n = 6 */
        cache_size = 4;
    }

    /* Number of cached elements needed for n
       index 0 corresponds to n = 3, so size = n-2   */
    idx_t needed = n - 2;
    if (needed > cache_size)
    {
        /* Grow the cache if necessary */
        if (needed > cache_cap)
        {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failure – unlikely in tests */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms using the recurrence relation */
        for (idx_t j = cache_size; j < needed; ++j)
        {
            /* cache[j] = a(j+3) = 17*a(j+2) - 88*a(j+1) + 153*a(j) - 81*a(j-1) */
            cache[j] = 17 * cache[j - 1]
                     - 88 * cache[j - 2]
                     + 153 * cache[j - 3]
                     - 81 * cache[j - 4];
        }
        cache_size = needed;
    }

    /* Return the desired value */
    return cache[n - 3];
}
