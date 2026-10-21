/*
 * A297852  Number of n X 2 0..1 arrays with every element equal to 1, 2 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-3)
   - 10*a(n-4) + 4*a(n-5) for n > 6
   with initial values a(1)=1, a(2)=3, a(3)=7, a(4)=13, a(5)=23, a(6)=49 */

/* Standard headers for memory functions.  They work for both C and C++. */
#include <stdlib.h>

/* Static cache for memoisation across calls.  cache[0] is unused. */
static seq_t *cache = NULL;     /* array of seq_t values, indices start at 1 */
static size_t cache_cap = 0;    /* current size of the allocated array (>=1) */

/* Compute a(idx_t n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Allocate the cache on first use. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* size 1 for index 0 */
        if (!cache) return 0;                     /* allocation failure */
        cache[0] = 0;
        cache_cap = 1;
    }

    /* Ensure we have space for index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;                     /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                                /* allocation failure */
        cache = tmp;
        /* Initialise new slots to 0 (uncomputed). */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n.  Terms that are already
       non‑zero are left untouched. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[(size_t)i] != 0)            /* already computed */
            continue;

        if (i == 1)            cache[(size_t)i] = 1;
        else if (i == 2)       cache[(size_t)i] = 3;
        else if (i == 3)       cache[(size_t)i] = 7;
        else if (i == 4)       cache[(size_t)i] = 13;
        else if (i == 5)       cache[(size_t)i] = 23;
        else if (i == 6)       cache[(size_t)i] = 49;
        else /* i >= 7 */ {
            cache[(size_t)i] = 3 * cache[(size_t)(i - 1)]
                            - 2 * cache[(size_t)(i - 2)]
                            + 4 * cache[(size_t)(i - 3)]
                            - 10 * cache[(size_t)(i - 4)]
                            + 4 * cache[(size_t)(i - 5)];
        }
    }

    return cache[(size_t)n];
}
