/*
 * A286910  Number of independent vertex sets and vertex covers in the
 *          n-antiprism graph.
 * https://oeis.org/A286910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) with a(0) = 3, a(1) = 1, a(2)
 *            = 5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A286910,
   the number of independent vertex sets and vertex covers
   in the n-antiprism graph. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   // number of entries currently allocated

    /* If we haven't computed up to n, grow the cache. */
    if (n >= cache_cap)
    {
        idx_t old_cap   = cache_cap;
        seq_t *tmp       = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)        /* allocation failed – return 0 (unlikely for test cases). */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = n + 1;

        /* Initialise the first three terms if they are within the new size. */
        if (cache_cap > 0) cache[0] = (seq_t)3;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        if (cache_cap > 2) cache[2] = (seq_t)5;

        /* Compute the recurrence for all missing terms. */
        idx_t start = (old_cap > 3) ? old_cap : 3;  /* start at 3 if we didn't have it yet */
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i)
        {
            cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
        }
    }

    return cache[n];
}
