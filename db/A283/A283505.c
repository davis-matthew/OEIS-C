/*
 * A283505  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3) for n>3 with a(0)=1,
 *            a(1)=0, a(2)=101, a(3)=11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoise computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* current capacity of cache */
    static idx_t max_n = 0;         /* largest n already computed */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 4;  /* up to index 3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 101;
        cache[3] = 11;
        max_n = 3;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1); /* fatal: allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t val = cache[i - 1]
                        + (seq_t)100 * cache[i - 2]
                        - (seq_t)100 * cache[i - 3];
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
