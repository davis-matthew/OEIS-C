/*
 * A393385  Number of minimal edge covers in the n-necklace graph.
 * https://oeis.org/A393385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1)+10*a(n-2)+6*a(n-3), with a(0)=3, a(1)=7, a(2)=69
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t cache_size = 0;     /* current size of the array */
    static idx_t max_computed = 0;   /* highest index that has been computed */

    /* Return small cases directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)69;

    /* Initialise cache if first call */
    if (cache == NULL) {
        cache_size = (n > 2 ? n : 2) + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)69;
        max_computed = 2;
    } else if (n >= cache_size) {
        /* Grow the cache */
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)cache_size * sizeof(seq_t));
    }

    /* If the requested value is already computed, simply return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute the recurrence for new indices */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1]
                 + (seq_t)10 * cache[i - 2]
                 + (seq_t)6  * cache[i - 3];
    }

    max_computed = n;
    return cache[n];
}
