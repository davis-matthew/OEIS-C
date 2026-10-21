/*
 * A248106  Least k such that ((k+1)/(k-1))^k - e^2 < 1/n^2.
 * https://oeis.org/A248106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   g_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = min k>=2 such that ((k+1)/(k-1))^k - e^2 < 1/n^2
 * (searched by binary search on the monotone decreasing function g(k))
 */
#include <math.h>
#include <stdint.h>
#include "../utils/g_value.h"

static const double E_SQUARED = 7.3890560989306495 /* e^2 */;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset unused */
    /* threshold: log(1 + 1/(n^2 e^2)) */
    double nd = (double)n;
    double r   = 1.0 / (nd * nd * E_SQUARED);
    double thresh = log1p(r);

    idx_t low  = 2;
    /* rough upper bound ≈ 2 e^2 n^2 + 10, avoid overflow on n^2 */
    idx_t high = (idx_t)(ceil(15.78 * nd * nd) + 10);
    if (high < low) high = low;

    /* ensure high actually satisfies the inequality */
    while (g_value(high) >= thresh) {
        high *= 2;
    }

    /* binary search for smallest k with g(k) < thresh */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (g_value(mid) < thresh) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}
