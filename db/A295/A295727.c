/*
 * A295727  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = 1, a(2) = 1, a(3) = 1.
 * https://oeis.org/A295727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = -1, a(1)
 *            = 1, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 45, i64 n <= 91, i128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* number of elements currently cached */

    /* initialise cache if first call */
    if (cap == 0)
    {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();              /* memory allocation failure */

        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
    }

    /* if requested index is already cached, return it */
    if (n < cap)
        return cache[n];

    /* enlarge cache if necessary */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < cap)
        new_cap = cap;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                  /* memory allocation failure */

    cache = new_cache;

    /* compute terms from current cap up to new_cap-1 */
    for (idx_t i = cap; i < (idx_t)new_cap; ++i)
    {
        cache[i] = cache[i - 1]
                 + 3 * cache[i - 2]
                 - 2 * cache[i - 3]
                 - 2 * cache[i - 4];
    }

    cap = (idx_t)new_cap;
    return cache[n];
}
