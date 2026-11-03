/*
 * A339377  Number of triples (x, y, z) of natural numbers satisfying x+y = n
 *          and 2*x*y = z^2.
 * https://oeis.org/A339377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * #{ d | 0 ≤ d ≤ n/2 , 2*d*(n-d) is a perfect square };
 *            (with a(0) = 1)
 * Range:     u8 n <= 11, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed values */
    static seq_t *cache = NULL;
    static idx_t last = 0;          /* largest n for which cache[i] is known */

    if (cache == NULL) {
        cache = mem_new(seq_t, (n + 1));
        if (cache == NULL) exit(EXIT_FAILURE);
        last = 0;                   /* nothing computed yet */
    } else if (n > last) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* compute any new values up to n */
    for (idx_t i = (last == 0 ? 0 : last + 1); i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;            /* only (0,0,0) */
            continue;
        }
        seq_t cnt = 0;
        for (idx_t d = 0; d <= i / 2; ++d) {
            seq_t v = (seq_t)2 * (seq_t)d * (seq_t)(i - d);
            seq_t s = floor_sqrt(v);
            if (s * s == v) ++cnt;
        }
        cache[i] = cnt * 2;          /* each unordered pair gives two ordered ones */
    }
    last = n;
    return cache[n];
}
