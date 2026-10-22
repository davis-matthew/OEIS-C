/*
 * A372970  a(1)=1, then a(n) = floor(n/max(a(n-1),a(floor(n/2)))).
 * https://oeis.org/A372970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1; for n >= 2,
 * a(n) = floor( n / max( a(n-1), a(floor(n/2)) ) )
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* input outside the defined range */

    /* Static cache with on‑demand expansion */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;         /* last index that has been computed */

    if (n <= last)                 /* already computed */
        return cache[n-1];

    /* Make sure the cache can hold at least n elements (1‑based) */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first term if this is the first call */
    if (last == 0) {
        cache[0] = (seq_t)1;        /* a(1) = 1 */
        last = 1;
    }

    /* Iteratively compute a(last+1) … a(n) */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t half = i >> 1;        /* floor(i/2) */
        seq_t prev = cache[i-2];    /* a(i-1) */
        seq_t halfv = cache[half-1];/* a(floor(i/2)) */
        seq_t denom = prev > halfv ? prev : halfv;
        seq_t num = (seq_t)i;
        cache[i-1] = num / denom;   /* floor division */
    }

    last = n;
    return cache[n-1];
}
