/*
 * A287051  a(0) = 0, a(1) = 1; a(2*n) = gpf(a(n)), a(2*n+1) = a(n) + a(n+1),
 *          where gpf(a(n)) is the greatest prime dividing a(n) for a(n) >= 2
 *          and 1 if a(n) = 1 (A006530).
 * https://oeis.org/A287051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = gpf(a(n)), a(2*n+1) = a(n) + a(n+1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   ensure_cache, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gpf.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can store up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing values sequentially */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if ((i & 1) == 0)   /* even index */
        {
            uint64_t val64 = (uint64_t)cache[i >> 1];
            uint64_t gp = gpf(val64);
            cache[i] = (seq_t)gp;
        } else                       /* odd index */
        {
            cache[i] = cache[i >> 1] + cache[(i >> 1) + 1];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
