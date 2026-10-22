/*
 * A398678  Number of spanning trees of the 3 X n grid graph that are
 *          invariant under reflection in the vertical axis.
 * https://oeis.org/A398678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398678: Number of spanning trees of the 3 × n grid graph
   invariant under reflection in the vertical axis.
   Recurrence: a(n) = 15*a(n-2) - 32*a(n-4) + 15*a(n-6) - a(n-8)
   Initial values: a(1)=1, a(2)=3, a(3)=8, a(4)=45, a(5)=95,
   a(6)=576, a(7)=1183, a(8)=7245 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i ≥ 1.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of entries allocated */
    static idx_t  cache_size = 0;  /* highest index computed */

    if (n < 1)
        return (seq_t)0;           /* undefined for n ≤ 0, return 0 */

    /* Grow the cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;        /* new_cap > n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            /* allocation failed */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)1;
        else if (i == 2)     cache[i] = (seq_t)3;
        else if (i == 3)     cache[i] = (seq_t)8;
        else if (i == 4)     cache[i] = (seq_t)45;
        else if (i == 5)     cache[i] = (seq_t)95;
        else if (i == 6)     cache[i] = (seq_t)576;
        else if (i == 7)     cache[i] = (seq_t)1183;
        else if (i == 8)     cache[i] = (seq_t)7245;
        else
            cache[i] = (seq_t)15 * cache[i-2]
                       - (seq_t)32 * cache[i-4]
                       + (seq_t)15 * cache[i-6]
                       - cache[i-8];
    }
    cache_size = n;

    return cache[n];
}
