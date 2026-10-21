/*
 * A298569  Number of nX2 0..1 arrays with every element equal to 0, 2, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-3) - 4*a(n-4), with a(1)=1, a(2)=4,
 *            a(3)=4, a(4)=8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return base cases directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)8;

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last index that has been computed */

    /* Extend cache if necessary. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – unlikely for test ranges. */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Ensure base values are set. */
        if (cache_size < 1) cache[1] = (seq_t)1;
        if (cache_size < 2) cache[2] = (seq_t)4;
        if (cache_size < 3) cache[3] = (seq_t)4;
        if (cache_size < 4) cache[4] = (seq_t)8;

        /* Compute new values using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 4) continue;  /* already initialized */
            cache[i] = cache[i - 1]
                       + (seq_t)8 * cache[i - 3]
                       - (seq_t)4 * cache[i - 4];
        }
        cache_size = n;
    }
    return cache[n];
}
