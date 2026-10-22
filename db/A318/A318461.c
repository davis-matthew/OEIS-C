/*
 * A318461  a(n) = Sum_{d|n, d <= n/d} (d OR n/d), where OR is bitwise-or
 *          (A003986).
 * https://oeis.org/A318461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, d <= n/d} (d OR n/d)
 * Range:     u8 n <= 107, u16 n <= 9205, u32 n <= 9205, u64 n <= 9205,
 *            u128 n <= 9205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit values */
static inline idx_t uint64_sqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid * mid <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return sum;
    idx_t lim = uint64_sqrt(n);
    for (idx_t d = 1; d <= lim; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            sum += ( (seq_t)d | (seq_t)q );
        }
    }
    return sum;
}
