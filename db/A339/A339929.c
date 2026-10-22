/*
 * A339929  a(n+1) = a(n-1-a(n)^2) + 1, starting with a(1) = a(2) = 0.
 * https://oeis.org/A339929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n-1 - a(n)^2) + 1, with a(1) = a(2) = 0. Defined for
 *            n >= 1, indexed by OEIS convention
 * Range:     u8 n <= 519, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache.  Indices 0..k are valid where k is the
       maximum n for which values have been computed.  Index 0 is unused
       but exists to keep the address arithmetic simple. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* items currently stored (0..cache_size-1) */

    /* Safety: a(n) is defined only for n >= 1. */
    if (n < 1)
        return 0;

    /* Grow the cache if the requested index is beyond the current size. */
    if (cache_size <= n) {
        /* New size is n+1 to allow indexing up to n inclusive. */
        idx_t new_size = n + 1;
        /* `realloc` returns void*; cast explicitly to satisfy C++. */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) {              /* allocation failed */
            /* In a real application you might abort or return an error. */
            return 0;
        }

        /* Compute any missing terms between the old size and n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 2) {
                cache[i] = 0;            /* a(1) = a(2) = 0 */
            } else {
                /* a(i) = a(i-2 - a(i-1)^2) + 1 */
                seq_t sq = cache[i - 1] * cache[i - 1];
                idx_t idx = (idx_t)(i - 2) - (idx_t)sq;
                cache[i] = cache[idx] + 1;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
