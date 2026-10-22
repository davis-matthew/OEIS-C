/*
 * A396748  G.f. A(x) satisfies A(x) = 1 + x * A(x^2/(1 - x)^2) / (1 - x)^3.
 * https://oeis.org/A396748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n+1, 2*k+2) * a(k),
 *            a(0)=1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoisation array */
    static size_t cache_cap = 0U;   /* allocated size of cache */

    /* Grow the cache so it contains indices 0…n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base case */
    if (cache[0] == 0)
        cache[0] = 1;

    /* Return the cached value if already known */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all terms up to n, skipping already computed ones */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)
            continue;

        idx_t kmax = (i - 1) / 2;
        seq_t sum = 0;

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t bin  = binomial(i + 1, 2 * k + 2);
            sum += bin * cache[k];
        }

        cache[i] = sum;
    }

    return cache[n];
}
