/*
 * A387612  a(n) = denominator(Integral_{(x,y,z) in R^3 : 0 <= x^2 + y^2 + z^2
 *          <= n^2}(x^2 + y^2) dx dy dz).
 * https://oeis.org/A387612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 15 / gcd(15, n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Local gcd for uint64_t values; small and safe. */

seq_t a(idx_t n)
{
    uint64_t g = gcd_uint64(15ULL, (uint64_t)n);
    return (seq_t)(15ULL / g);
}
