/*
 * A275019  2-adic valuation of tetrahedral numbers C(n+2,3) = n(n+1)(n+2)/6 =
 *          A000292(n).
 * https://oeis.org/A275019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(n)+A007814(n+1)+A007814(n+2)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A007814(k) is the exponent of 2 dividing k (v₂(k)). */

#include <stdint.h>
#include "../utils/tz.h"

seq_t a(idx_t n)
{
    seq_t v1 = tz(n);
    seq_t v2 = tz(n + 1);
    seq_t v3 = tz(n + 2);
    return v1 + v2 + v3 - 1;
}
