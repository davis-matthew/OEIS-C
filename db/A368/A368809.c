/*
 * A368809  Number of 4 X n binary arrays with a path of adjacent 1's from top
 *          row to bottom row using only left, right, and downward steps.
 * https://oeis.org/A368809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 36*a(n-1) - 413*a(n-2) + 1642*a(n-3)
                 - 2536*a(n-4) + 1152*a(n-5)
   Initial values: a(1)=1, a(2)=41, a(3)=1041, a(4)=22193, a(5)=433801 */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialise cache with the first five terms */
    if (!cache) {
        cache_cap = 8;                                    /* spare space */
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation failure */

        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)41;         /* a(2) */
        cache[2] = (seq_t)1041;       /* a(3) */
        cache[3] = (seq_t)22193;      /* a(4) */
        cache[4] = (seq_t)433801;     /* a(5) */
        cache_len = 5;
    }

    /* Grow the cache array if the requested term is beyond current capacity */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* realloc failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing values up to n using the linear recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i starts from 6 since the recurrence applies from a(6) onwards */
        seq_t val = (seq_t)36  * cache[i - 2]
                   - (seq_t)413 * cache[i - 3]
                   + (seq_t)1642 * cache[i - 4]
                   - (seq_t)2536 * cache[i - 5]
                   + (seq_t)1152 * cache[i - 6];
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
