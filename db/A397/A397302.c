/*
 * A397302  Number of ways to tile a strip of length n with three types of
 *          tiles.
 * https://oeis.org/A397302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 2*a(n-3), a(0) = 1, a(1) = 2, a(2)
 *            = 10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation array */
    static size_t cap   = 0;       /* number of elements allocated */

    size_t idx = (size_t)n;        /* convert index to a size_t */

    if (idx >= cap)               /* cache too small – grow it */
    {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                 /* realloc failed – unlikely in tests */
            return (seq_t)0;
        cache = tmp;

        /* Initialise base values (if not already present) */
        if (cap == 0) {
            cache[0] = 1;
            if (new_cap > 1) cache[1] = 2;
            if (new_cap > 2) cache[2] = 10;
        } else {
            if (new_cap > 1) cache[1] = 2;
            if (new_cap > 2) cache[2] = 10;
        }

        /* Compute missing terms using the recurrence */
        size_t start = (cap > 3 ? cap : 3);
        for (size_t i = start; i <= idx; ++i)
            cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] + 2 * cache[i - 3];

        cap = new_cap;
    }

    return cache[idx];
}
