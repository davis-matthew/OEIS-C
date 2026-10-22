/*
 * A353100  a(1) = 8; for n>1, a(n) = 9 * a(n-1) + 9 - n.
 * https://oeis.org/A353100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 8; for n>1, a(n) = 9*a(n-1) + 9 - n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to avoid recomputation for repeated calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_comp = 0;

    if (n == 0) return 0;  /* outside the defined offset; return 0 */

    /* Extend cache storage if needed */
    if ((size_t)(n + 1) > cache_cap) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Initialise first term if not done yet */
    if (max_comp < 1) {
        cache[1] = (seq_t)8;
        max_comp = 1;
    }

    /* Compute missing terms using the recurrence relation */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = (seq_t)9 * cache[i - 1] + (seq_t)9 - (seq_t)i;
    }
    max_comp = n;

    return cache[n];
}
