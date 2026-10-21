/*
 * A254895  Indices of octagonal numbers (A000567) that are also centered
 *          square numbers (A001844).
 * https://oeis.org/A254895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n−1)+98*a(n−2)−98*a(n−3)−a(n−4)+a(n−5), with a(1)=1,
 *            a(2)=13, a(3)=53, a(4)=1241, a(5)=5161, n≥1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;      /* array of computed terms, 0‑unused */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t  cache_len = 0;     /* highest index computed (≤ cache_len < cache_cap) */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  For n==0 return 0. */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(6, &cache, &cache_cap);   /* indices 0..5 */
        cache[0] = 0;   /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)53;
        cache[4] = (seq_t)1241;
        cache[5] = (seq_t)5161;
        cache_len = 5;
    }

    if (n <= cache_len) return cache[n];

    /* compute terms up to n, storing in cache */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if ((size_t)i + 1 > cache_cap)
            ensure_cache((size_t)i + 1, &cache, &cache_cap);

        seq_t val = cache[i-1];
        val += (seq_t)98 * cache[i-2];
        val -= (seq_t)98 * cache[i-3];
        val -= cache[i-4];
        val += cache[i-5];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
