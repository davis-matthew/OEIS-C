/*
 * A295674  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 2, a(2) =
 *          4, a(3) = 8.
 * https://oeis.org/A295674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) a(0) = 1, a(1) = 2, a(2) = 4,
 *            a(3) = 8
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization, zero‑based */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;            /* number of slots currently allocated */

    /* if we already have the value return it */
    if (n < cache_size)
        return cache[n];

    /* need to grow the table up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)                     /* allocation failed – return 0 as a graceful fallback */
        return 0;
    cache = tmp;

    /* remember how much was allocated before the resize – the index from which
       new values must be computed */
    idx_t old_size = cache_size;
    cache_size = new_size;

    /* initialise the first four terms if they are within the new table */
    if (old_size <= 0 && cache_size >= 1) cache[0] = (seq_t)1;
    if (old_size <= 1 && cache_size >= 2) cache[1] = (seq_t)2;
    if (old_size <= 2 && cache_size >= 3) cache[2] = (seq_t)4;
    if (old_size <= 3 && cache_size >= 4) cache[3] = (seq_t)8;

    /* compute any missing terms up to n */
    for (idx_t i = old_size; i <= n; ++i)
    {
        if (i <= 3)
        {
            /* base values already set above – keep them */
            continue;
        }
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }

    return cache[n];
}
