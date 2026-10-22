/*
 * A359479  a(1) = 1; a(n) = Sum_{k=2..n} (-1)^k * k * a(floor(n/k)).
 * https://oeis.org/A359479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n) = Σ_{k=2..n} (-1)^k * k * a(⌊n/k⌋)
 * Range:     i8 n <= 27, i16 n <= 447, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_max = 0;

    /* Ensure we have room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n];

    /* Compute all missing entries up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 2; k <= i; ++k) {
            idx_t m = i / k;
            seq_t prod = (seq_t)k * cache[m];
            if (k & 1)
                sum -= prod;   /* sign = -1 for odd k */
            else
                sum += prod;   /* sign = +1 for even k */
        }
        cache[i] = sum;
    }
    cache_max = n;
    return cache[n];
}
