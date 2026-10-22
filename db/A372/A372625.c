/*
 * A372625  Expansion of Sum_{k>=1} k^2 * x^(k^2) / (1 + x^k).
 * https://oeis.org/A372625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n, d≤√n} (-1)^(d + n/d) * d^2
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t (floor). */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    if (high > n) high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard anyway */
    seq_t sum = 0;
    idx_t root = _isqrt_u64(n);
    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t partner = n / d;
            uint8_t parity = (unsigned)(d + partner) & 1U;
            seq_t d2 = (seq_t)d * (seq_t)d;
            sum += (parity ? -d2 : d2);
        }
    }
    return sum;
}
