/*
 * A269556  Expansion of (-7*x^2 + 148*x - 5)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) Initial values: a(0) = 5,
 *            a(1) = 347, a(2) = 33865
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache to avoid recomputing the same terms */
    static seq_t *cache   = NULL; /* array of computed terms */
    static size_t cache_cap = 0;  /* current size of cache */

    /* initialise cache with the first three values */
    if (cache_cap == 0)
    {
        cache_cap = 3;
        cache = (seq_t*)malloc(sizeof(seq_t)*cache_cap);
        if (!cache) return 0;            /* allocation failure -> 0 */
        cache[0] = 5;
        cache[1] = 347;
        cache[2] = 33865;
    }

    /* extend cache if n is beyond current capacity */
    if ((idx_t)n >= (idx_t)cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
        if (!tmp) return 0;              /* allocation failure -> 0 */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (size_t i = cache_cap; i <= (size_t)n; ++i)
        {
            cache[i] = 99*cache[i-1] - 99*cache[i-2] + cache[i-3];
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
