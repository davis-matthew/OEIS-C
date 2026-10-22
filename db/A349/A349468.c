/*
 * A349468  a(n) = (4*n)! / (n! * (2*n)!).
 * https://oeis.org/A349468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)! / (n! * (2*n)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using recurrence: a(0)=1,  a(n) = a(n-1) * 4*(4*n-1)*(4*n-3) / n for n>=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache is large enough */
    if (n + 1 > cache_len) {
        /* Allocate or extend the cache */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) /* allocation failure – abort gracefully */
            abort();
        cache = new_cache;

        /* If cache was empty, initialise a(0) */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t numerator = (seq_t)4 * (seq_t)(4 * i - 1) * (seq_t)(4 * i - 3);
            cache[i] = (cache[i - 1] * numerator) / (seq_t)i;
        }

        cache_len = new_len;
    }

    return cache[n];
}
