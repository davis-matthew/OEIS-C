/*
 * A398664  Upper (3/2, 1) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6) initial values: a(0)=0, a(1)=3, a(2)=10, a(3)=23,
 *            a(4)=45, a(5)=78
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* base values */
    static const seq_t init[6] = {0, 3, 10, 23, 45, 78};

    /* if n is within the known initial block */
    if (n < 6)
        return init[(size_t)n];

    /* allocate cache if this is the first call */
    if (!cache) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* grow the cache if needed and compute missing terms */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                4 * cache[i - 1]
                - 7 * cache[i - 2]
                + 8 * cache[i - 3]
                - 7 * cache[i - 4]
                + 4 * cache[i - 5]
                - cache[i - 6];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
