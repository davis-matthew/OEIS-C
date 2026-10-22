/*
 * A320468  a(n) = a(n-1) + 20*a(n-2), n >= 2; a(0)=1, a(1)=41.
 * https://oeis.org/A320468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=41, a(n)=a(n-1)+20*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return precomputed value if available */
    if (n < cache_size)
        return cache[n];

    /* Expand cache to size n+1 */
    idx_t new_size = n + 1;
    if (!cache)
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else if (new_size > cache_size)
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Initialise base cases if not already done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)41;
        cache_size = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 1] + ((seq_t)20) * cache[i - 2];
    }
    cache_size = n + 1;
    return cache[n];
}
