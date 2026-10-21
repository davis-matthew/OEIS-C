/*
 * A254601  Numbers of n-length words on alphabet {0,1,...,6} with no subwords
 *          ii, where i is from {0,1,2}.
 * https://oeis.org/A254601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper to grow the cache array when required.
   It doubles the capacity until it can hold indices up to "n". */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    size_t needed = (size_t)n + 1;
    if (*cap_ptr < needed)
    {
        size_t new_cap = *cap_ptr ? *cap_ptr * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
        if (new_arr)
        {
            *cache_ptr = new_arr;
            *cap_ptr = new_cap;
        }
    }
}

/* Computes the n-th term of A254601. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..last] holds computed values */
    static size_t cap   = 0;       /* allocated capacity */
    static size_t last  = 0;       /* highest index computed */
    static int initialized = 0;

    /* First call: initialise cache for indices 0 and 1. */
    if (!initialized)
    {
        ensure_cache(1, &cache, &cap);   /* need space for indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        last    = 1;
        initialized = 1;
    }

    /* Return early if the requested index is already cached. */
    if ((size_t)n <= last)
        return cache[n];

    /* Grow the cache if necessary and compute missing terms. */
    ensure_cache((idx_t)n, &cache, &cap);

    for (size_t i = last + 1; i <= (size_t)n; ++i)
        cache[i] = cache[i - 1] * (seq_t)6 + cache[i - 2] * (seq_t)4;

    last = (size_t)n;
    return cache[n];
}
