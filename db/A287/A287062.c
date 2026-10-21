/*
 * A287062  Number of dominating sets in the 2n-crossed prism graph.
 * https://oeis.org/A287062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1)+31*a(n-2)+21*a(n-3)+2*a(n-4) * with initial
 *            terms a(1)=11, a(2)=183, a(3)=2417, a(4)=32499
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* n must be at least 1; for n==0 return 0 (undefined in OEIS). */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation cache.  Indices 0..cap-1. */
    static seq_t *cache   = NULL;   /* array of cached values */
    static size_t cap    = 0;        /* current capacity of cache */
    static idx_t  max_n  = 0;        /* greatest n computed so far */

    /* Initialise cache with the first four terms. */
    if (cache == NULL) {
        cap = 5; /* store indices 0..4; index 0 unused */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            exit(1); /* allocation failure */
        /* seeds (index 1‑4) */
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)183;
        cache[3] = (seq_t)2417;
        cache[4] = (seq_t)32499;
        max_n  = 4;
    }

    /* Grow cache if required to hold index n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1); /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Cast indices to size_t for array access. */
            cache[(size_t)i] =
                11 * cache[(size_t)(i - 1)] +
                31 * cache[(size_t)(i - 2)] +
                21 * cache[(size_t)(i - 3)] +
                2  * cache[(size_t)(i - 4)];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
