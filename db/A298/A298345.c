/*
 * A298345  a(n) = a(n-1) + a(n-2) + a([n/3]) + a([2n/3]), where a(0) = 1,
 *          a(1) = 2, a(2) = 3.
 * https://oeis.org/A298345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a([n/3]) + a([2n/3]), a(0)=1, a(1)=2,
 *            a(2)=3
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 40, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store already computed terms. */
    static seq_t *cache = NULL;   /* pointer to the cache array */
    static idx_t cache_size = 0;  /* number of elements currently stored   */

    /* Expand the cache if necessary. */
    if (n + 1 > cache_size) {
        /* Allocate at least one more element, double the size otherwise. */
        idx_t new_size = (n + 1 > cache_size * 2) ? n + 1 : cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* If we cannot allocate more memory we abort – the sequence
               must be computable in the first place. */
            abort();
        }
        cache = tmp;

        /* Compute from the current cache size up to n (inclusive). */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i == 1) {
                val = (seq_t)2;
            } else if (i == 2) {
                val = (seq_t)3;
            } else {
                val = cache[i - 1] + cache[i - 2] + cache[i / 3] + cache[(i * 2) / 3];
            }
            cache[i] = val;
        }
        /* Update the cache size. */
        if (cache_size < n + 1)
            cache_size = n + 1;
    }

    return cache[n];
}
