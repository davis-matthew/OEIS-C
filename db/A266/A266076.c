/*
 * A266076  Number of n X 3 integer arrays with each element equal to the
 *          number of horizontal and vertical neighbors differing from itself
 *          by exactly one.
 * https://oeis.org/A266076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 59, u64 n <= 117,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266076
   Recurrence:  a(n) = a(n-1) + a(n-3) + a(n-6) - a(n-7) - a(n-9)
   Initial values:  a(1)=2, a(2)=4, a(3)=4, a(4)=6, a(5)=5, a(6)=12,
                    a(7)=10, a(8)=16, a(9)=21
   Offset: 1   (a(1) is the first term)
*/
#include <stddef.h>
#include <stdlib.h>

/* Method to compute A266076, valid for any seq_t and idx_t
   provided by the build system. The function is memoised so that
   repeated calls for arbitrary indices, in any order, are efficient.
*/
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;      /* allocated elements */
    static size_t cache_size = 0;      /* highest index computed + 1 */

    /* Initial values for indices 1..9 (index 0 unused). */
    static const seq_t init[10] = {
        0,   /* 0 unused */
        2,   /* a(1) */
        4,   /* a(2) */
        4,   /* a(3) */
        6,   /* a(4) */
        5,   /* a(5) */
        12,  /* a(6) */
        10,  /* a(7) */
        16,  /* a(8) */
        21   /* a(9) */
    };

    /* For n < 10 we can return the constant directly. */
    if ((size_t)n < 10)
        return init[(size_t)n];

    /* Ensure the cache can hold values up to n.  Use exponential
       growth to keep amortised complexity O(n). */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failed; return zero as a safe error value */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute any values that have not yet been memoised. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 10) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1] + cache[i-3] + cache[i-6]
                       - cache[i-7] - cache[i-9];
        }
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
