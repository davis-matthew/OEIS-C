/*
 * A255236  All positive solutions x of the second class of the Pell equation
 *          x^2 - 2*y^2 = -7.
 * https://oeis.org/A255236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255236: all positive solutions x of x^2 – 2y^2 = –7
   Formula (recurrence):  a(0) = 5, a(1) = 31,
   a(n) = 6*a(n-1) – a(n-2)   for n ≥ 2             */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t last_index = (idx_t)-1;   /* no terms computed yet */

    /* ------------------------------------------
       Initialise cache with the two starting terms
       ------------------------------------------ */
    if (cache == NULL) {
        cache_capacity = 2;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed – return 0 */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)31;
        last_index = 1;
    }

    /* ------------------------------------------
       For indices 0 or 1, return the stored values
       ------------------------------------------ */
    if (n <= 1) return cache[(size_t)n];

    /* ------------------------------------------
       Grow cache if required
       ------------------------------------------ */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed – return 0 */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* ------------------------------------------
       Compute missing terms up to n using the recurrence
       ------------------------------------------ */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)6 * cache[(size_t)(i - 1)] - cache[(size_t)(i - 2)];
    }
    last_index = n;

    return cache[(size_t)n];
}
