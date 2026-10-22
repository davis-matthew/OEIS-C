/*
 * A301836  Number of n X 3 0..1 arrays with every element equal to 0, 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2), n > 3 Initial terms: a(1) = 4, a(2)
 *            = 25, a(3) = 139 Implemented with memoized recurrence, O(n) in
 *            time and space
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behavior below offset is irrelevant */

    /* Static storage for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated size */
    static idx_t current_max = 0;    /* highest n already computed */

    /* Initial allocation and base values */
    if (cache == NULL)
    {
        /* Ensure room for at least the first three terms */
        cache_cap = (n > 3 ? n : 4);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0; /* allocation failed – unlikely */

        cache[0] = 0;                /* unused */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)139;
        current_max = 3;
    }

    /* Grow the cache if needed */
    if (cache_cap <= n)
    {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return 0;             /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (current_max < n)
    {
        for (idx_t i = current_max + 1; i <= n; ++i)
        {
            seq_t t1 = (seq_t)7 * cache[i - 1];
            seq_t t2 = (seq_t)8 * cache[i - 2];
            cache[i] = t1 - t2;
        }
        current_max = n;
    }

    return cache[n];
}
