/*
 * A333760  Number of self-avoiding closed paths in the 4 X n grid graph which
 *          pass through all vertices on four (left, right, upper, lower)
 *          sides of the graph.
 * https://oeis.org/A333760
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 3*a(n-3) + a(n-4) for n > 5 Initial
 *            values: a(2)=1, a(3)=3, a(4)=11, a(5)=36 Offset: n >= 2 (a(2) is
 *            first term)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behavior is unspecified */
    if (n < 2) return (seq_t)0;

    /* Memoization cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of allocated slots */
    static idx_t last_computed = 0;  /* biggest index computed so far */

    /* Allocate or grow the cache to accommodate n */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* out of memory – return 0 */
        cache = new_cache;
        cache_size = (idx_t)new_size;
    }

    /* Compute values only up to the requested index n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i < 2) {
            val = (seq_t)0;
        } else if (i == 2) {
            val = (seq_t)1;
        } else if (i == 3) {
            val = (seq_t)3;
        } else if (i == 4) {
            val = (seq_t)11;
        } else if (i == 5) {
            val = (seq_t)36;
        } else { /* i >= 6 */
            val = (seq_t)(3 * cache[i - 1] +
                          2 * cache[i - 2] -
                          3 * cache[i - 3] +
                          cache[i - 4]);
        }
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
