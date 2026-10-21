/*
 * A240944  Number of compositions of n into square parts k^2 where there are
 *          k sorts of part k^2.
 * https://oeis.org/A240944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k^2 <= n} k * a(n - k^2), with a(0) = 1. This
 *            implements the generating‑function recurrence for the
 *            compositions into square parts with k sorts for part k^2
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 50, u64 n <= 98, u128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* assume seq_t and idx_t are defined elsewhere by the build system */
seq_t a(idx_t n)
{
    /* static cache of already computed values, indexed by n.
       cache_len is the number of allocated entries (>= computed indices). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure the cache can hold indices up to n. */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;               /* number of slots needed */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                          /* out of memory → 0 */
        /* initialize the new slots to 0 */
        for (size_t i = (size_t)cache_len; i < new_len; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_len = (idx_t)new_len;
    }

    /* initialise a(0) if it hasn't been set yet */
    if (cache[0] == 0)
        cache[0] = 1;

    /* compute values up to n, if not already cached */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;                 /* already computed */
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t k2 = k * k;
            if (k2 > i) break;
            sum += cache[i - k2] * (seq_t)k;
        }
        cache[i] = sum;
    }

    return cache[n];
}
