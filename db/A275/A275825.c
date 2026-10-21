/*
 * A275825  Third-order sequence with non-constant coefficients: a(n) =
 *          (n-3)*a(n-1) + (n-1)*a(n-3); a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A275825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-3)*a(n-1) + (n-1)*a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;

    /* Ensure the cache can address indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Set up initial values once. */
    if (cache_len < 3) {
        for (size_t i = 0; i < 3; ++i)
            cache[i] = (seq_t)1;
        cache_len = 3;
    }

    /* Compute any terms that have not yet been computed. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)(i - 3)) * cache[i - 1] +
                   ((seq_t)(i - 1)) * cache[i - 3];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
