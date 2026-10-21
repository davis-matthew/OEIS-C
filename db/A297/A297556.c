/*
 * A297556  a(n) = a(n-1) + 16*a(n-3) - 16*a(n-4), where a(0) = 1, a(1) = 4,
 *          a(2) = 7, a(3) = 19.
 * https://oeis.org/A297556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 16*a(n-3) - 16*a(n-4)   (n ≥ 4)
   a(0) = 1,  a(1) = 6,  a(2) = 7,  a(3) = 19
   This implementation memoises computed terms in a dynamic array.
   It works for seq_t of any unsigned integer type or arbitrary‑precision
   integer class.  The code is valid C11 and C++17. */
#include <stdlib.h>

static void grow_cache(idx_t need, seq_t **cache, size_t *cap)
{
    /* need is the maximum index that must be reachable (0‑based) */
    size_t required = (size_t)need + 1;           /* number of elements needed */
    if (*cap < required)                           /* sparse → reallocate */
    {
        size_t new_cap = required;
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cache = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cap       = 0;                  /* capacity in number of elements */
    static idx_t max_comp   = (idx_t)-1;          /* largest index computed */

    /* initialise on first call */
    if (max_comp == (idx_t)-1)
    {
        grow_cache(3, &cache, &cap);              /* space for indices 0..3 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)19;
        max_comp = 3;
    }

    if (n <= max_comp)
        return cache[(size_t)n];

    /* extend storage so that index n is reachable */
    grow_cache(n, &cache, &cap);

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i)
        cache[(size_t)i] = cache[(size_t)(i - 1)]
                       + (seq_t)16 * cache[(size_t)(i - 3)]
                       - (seq_t)16 * cache[(size_t)(i - 4)];

    max_comp = n;
    return cache[(size_t)n];
}
