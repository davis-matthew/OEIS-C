/*
 * A314219  Coordination sequence Gal.5.314.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + a(n-3) + a(n-11) - 2*a(n-12) * +
 *            2*a(n-13) - a(n-14) for n > 14. * Initial values: a(0)=1,
 *            a(1)=5, a(2)=11, a(3)=17, a(4)=22, a(5)=28, * a(6)=34, a(7)=38,
 *            a(8)=41, a(9)=47, a(10)=55, a(11)=62, a(12)=67, * a(13)=71,
 *            a(14)=75
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"


seq_t a(idx_t n)
{
    static const seq_t base[15] = {
        (seq_t)1,  (seq_t)5,  (seq_t)11, (seq_t)17, (seq_t)22,
        (seq_t)28, (seq_t)34, (seq_t)38, (seq_t)41, (seq_t)47,
        (seq_t)55, (seq_t)62, (seq_t)67, (seq_t)71, (seq_t)75
    };

    if (n < 15) {
        return base[n];
    }

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_filled = 0;

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Fill base values if they are not yet in the cache. */
    if (last_filled < 14) {
        for (idx_t i = last_filled + 1; i <= 14; ++i) {
            cache[i] = base[i];
        }
        last_filled = 14;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1]
                 - 2 * cache[i - 2]
                 + cache[i - 3]
                 + cache[i - 11]
                 - 2 * cache[i - 12]
                 + 2 * cache[i - 13]
                 - cache[i - 14];
    }

    last_filled = n;
    return cache[n];
}
