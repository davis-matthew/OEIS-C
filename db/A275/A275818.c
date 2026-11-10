/*
 * A275818  Smallest number k such that k^2 matches the conditions of A275817.
 * https://oeis.org/A275818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(s*sqrt(n)) + 1, where s = A275817(n) is the least s
 *            with a square k^2 in (s^2*n, s^2*(n+1)); found by search over s
 *            (a(0) = 1)
 * Range:     u8 n <= 7, u16 n <= 501, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 966 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A275818_isqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    /* invariant: lo*lo <= x < hi*hi */
    while (hi - lo > 1) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    for (seq_t s = 1; ; ++s) {
        seq_t s2 = s * s;
        seq_t lowv = s2 * nn;
        seq_t k = A275818_isqrt(lowv) + 1;
        if (k * k < s2 * (nn + 1))
            return k;
    }
}
