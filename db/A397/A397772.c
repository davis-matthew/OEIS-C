/*
 * A397772  Inverse Möbius transform of A351830.
 * https://oeis.org/A397772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} A351830(d), where A351830(d) = distance from
 *            d(d+1)(2d+1)/6 to the nearest square
 * Range:     u8 n <= 57, u16 n <= 1889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9431 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 u128_A397772;

static u128_A397772 A397772_isqrt(u128_A397772 x)
{
    u128_A397772 low = 0, high = x, mid, res = 0;
    if (high > (u128_A397772)0xFFFFFFFFFFFFFFFFULL) high = (u128_A397772)0xFFFFFFFFFFFFFFFFULL;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid * mid <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

static uint64_t A397772_term(uint64_t d)
{
    u128_A397772 nn = (u128_A397772)d;
    u128_A397772 p = nn * (nn + 1) * (2 * nn + 1) / 6;
    u128_A397772 s = A397772_isqrt(p);
    u128_A397772 d1 = p - s * s;
    u128_A397772 d2 = (s + 1) * (s + 1) - p;
    return (uint64_t)(d1 <= d2 ? d1 : d2);
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    uint64_t m = (uint64_t)n;
    uint64_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            total += (seq_t)A397772_term(d);
            if (d * d != m) total += (seq_t)A397772_term(m / d);
        }
    }
    return total;
}
