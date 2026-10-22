/*
 * A332145  a(n) = 4*(10^(2*n+1)-1)/9 + 10^n.
 * https://oeis.org/A332145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Recurrence: a(0)=5, a(1)=454, a(2)=44544; *
 *            a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) for n>2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms already stored   */
    static size_t cache_cap = 0;  /* allocated capacity in terms       */

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow the cache so it can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);   /* guarantees n+1 slots */

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 5;
        } else if (i == 1) {
            val = 454;
        } else if (i == 2) {
            val = 44544;
        } else {
            /* recurrence: a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) */
            val = 111 * cache[i - 1] - 1110 * cache[i - 2] + 1000 * cache[i - 3];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
