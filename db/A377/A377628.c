/*
 * A377628  a(n) = a(n-1) + a(n-2) + 1 with a(0)=2 and a(1)=2.
 * https://oeis.org/A377628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 (a(0)=2, a(1)=2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)2;  /* a(0) = a(1) = 2 */

    if (cache_size == 0) {
        /* initialise cache with the first two values */
        cache_size = 2;
        cache = seq_alloc(cache_size);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
    }

    if (n >= cache_size) {
        /* grow the cache to accommodate up to n */
        seq_t *new_cache = seq_realloc(cache, cache_size, n + 1);
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + 1;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
