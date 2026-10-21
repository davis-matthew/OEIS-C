/*
 * A262677  Number of odd numbers encountered when iterating A049820 starting
 *          from n: a(0) = 0 and for n >= 1, a(n) = A000035(n) +
 *          a(A049820(n)).
 * https://oeis.org/A262677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = is_odd(n) + a(n - d(n)), a(0) = 0 where d(n) =
 *            divisor_count_u64(n) (OEIS A000005). The algorithm builds the
 *            values iteratively from 0 up to the requested n, using a static
 *            cache that is expanded on demand
 * Range:     u8 n <= 3496, u16 n <= 9969, u32 n <= 9969, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   divisor_count_u64, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/divisor_count_u64.h" /* divisor_count_u64(uint64_t) */
#include "../utils/ensure_cache.h"      /* ensure_cache(idx_t, seq_t**, size_t*) */

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i <= cache_max   */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_max  = 0;

    /* make sure we have room for indices 0..n   */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise a(0) – harmless to assign each time   */
    cache[0] = 0;

    /* compute values that have not yet been cached   */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* n - d(n)  (always < i for i >= 1)  */
        idx_t step = (idx_t)divisor_count_u64(i);
        idx_t next = i - step;          /* guaranteed non‑negative           */

        /* a(i) = [i odd] + a(next)     */
        cache[i] = ((i & 1) ? (seq_t)1 : (seq_t)0) + cache[next];
    }

    cache_max = n;
    return cache[n];
}
