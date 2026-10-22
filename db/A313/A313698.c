/*
 * A313698  Coordination sequence Gal.6.157.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) * - 4*a(n-8) + 3*a(n-9) - a(n-10), n > 10 * Initial
 *            values: a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=19, * a(5)=24,
 *            a(6)=30, a(7)=36, a(8)=41, a(9)=45, a(10)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init[11] = { 1, 5, 10, 15, 19, 24, 30, 36, 41, 45, 50 };
/* Static cache big enough for the required n (≤10000) */
static seq_t cache[10001];
static bool cache_filled = false;
static size_t cache_size = 0;

static void init_cache(void) {
    if (!cache_filled) {
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_size = 11;
        cache_filled = true;
    }
}

seq_t a(idx_t n) {
    init_cache();                  /* ensure base values are in place */

    if ((idx_t)n < cache_size)
        return cache[n];

    /* Compute values up to n (the recurrence needs values up to n-10) */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* safe because k >= 11 once this loop runs */
        seq_t pos = cache[k-1] * 3 + cache[k-3] * 3 + cache[k-7] * 3 + cache[k-9] * 3;
        seq_t neg = cache[k-2] * 4 + cache[k-4] + cache[k-6] + cache[k-8] * 4 + cache[k-10];
        cache[k] = pos - neg;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
