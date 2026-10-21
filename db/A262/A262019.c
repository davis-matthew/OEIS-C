/*
 * A262019  The first of eleven consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of five
 *          consecutive positive integers.
 * https://oeis.org/A262019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A262019: First of eleven consecutive positive integers
   whose squares sum equals the sum of the squares of five consecutive
   positive integers.

   Recurrence (n > 3):
       a(n) = 179*a(n-1) - 179*a(n-2) + a(n-3)

   Initial values:
       a(1) = 15,   a(2) = 3575,   a(3) = 637215 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 in that case. */
    if (n < 1) return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache   = NULL; /* cache[0] is unused */
    static size_t cache_cap = 0;
    static idx_t   max_n   = 0;   /* greatest index computed so far */

    /* Initialise the cache with the first three values. */
    if (!cache) {
        cache_cap = 3;                          /* indices 1..3 exist */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache[0] = (seq_t)0;                    /* unused */
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)3575;
        cache[3] = (seq_t)637215;
        max_n   = 3;
    }

    /* Extend the cache if needed. */
    if (n > max_n) {
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[(size_t)i] = (seq_t)179 * cache[(size_t)(i-1)]
                              - (seq_t)179 * cache[(size_t)(i-2)]
                              +          cache[(size_t)(i-3)];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
