/*
 * A385555  Period of {binomial(N,3) mod n: N in Z}.
 * https://oeis.org/A385555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(6, n)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t g  = gcd(sn, (seq_t)6);
    return sn * g;
}
