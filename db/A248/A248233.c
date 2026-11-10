/*
 * A248233  Numbers k such that A248231(k+1) = A248231(k) + 1.
 * https://oeis.org/A248233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n^2 + (n+1)^2)) = floor(sqrt(2n^2+2n+1))
 *            (duplicate of A097432)
 * Range:     u8 n <= 10, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 935 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t val = sn * (sn + 1) * 2 + 1;
    /* n <= sqrt(val) < 2n for n >= 1; find the largest m with m <= val/m */
    seq_t lo = sn, hi = sn * 2;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= val / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}
