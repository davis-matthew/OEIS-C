/*
 * A269613  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by other than plus two, zero or
 *          minus 1.
 * https://oeis.org/A269613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3), a(1)=3, a(2)=9, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Static cache for memoization */
    static seq_t *cache   = NULL;        /* array of a(i) for i <= max_comp */
    static idx_t cache_cap = 0;          /* allocated size of cache  */
    static idx_t max_comp  = 0;          /* largest n that has been computed */

    /* Allocate initial cache and fill base values */
    if (!cache) {
        cache_cap = 4;                   /* indices 0..3, we ignore index 0 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Base cases */
        cache[(idx_t)1] = (seq_t)3;
        cache[(idx_t)2] = (seq_t)9;
        cache[(idx_t)3] = (seq_t)27;
        max_comp = 3;
    }

    /* If n already computed, return it */
    if (n <= max_comp)
        return cache[n];

    /* Ensure cache is large enough */
    if ((n + (idx_t)1) > cache_cap) {
        cache_cap = n + (idx_t)1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing entries using the recurrence */
    for (idx_t i = max_comp + (idx_t)1; i <= n; ++i) {
        cache[i] = (seq_t)(4 * cache[i-1] - 2 * cache[i-2] - 4 * cache[i-3]);
    }

    max_comp = n;
    return cache[n];
}
