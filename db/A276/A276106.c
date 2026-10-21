/*
 * A276106  Number of compositions of n into parts 1, 7, and 8.
 * https://oeis.org/A276106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) + a(n-8) for n > 7 Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=1, a(7)=2,
 *            a(8)=4
 * Range:     u8 n <= 23, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Memoization cache. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* cache capacity (number of slots allocated) */
    static size_t cache_size = 0; /* highest computed index + 1 */

    /* Ensure we have space for index n. */
    if ((idx_t)cache_cap <= n)
        ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing values up to n. */
    if ((idx_t)cache_size <= n) {
        idx_t i;
        for (i = (idx_t)cache_size; i <= n; ++i) {
            if (i <= 6) {
                cache[(size_t)i] = (seq_t)1;
            } else if (i == 7) {
                cache[(size_t)i] = (seq_t)2;
            } else if (i == 8) {
                cache[(size_t)i] = (seq_t)4;
            } else {
                cache[(size_t)i] =
                    cache[(size_t)(i-1)] + cache[(size_t)(i-7)] + cache[(size_t)(i-8)];
            }
        }
        cache_size = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
