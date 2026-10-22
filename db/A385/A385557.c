/*
 * A385557  Period of {binomial(N,5) mod n: N in Z}. Also, period of
 *          {binomial(N,6) mod n: N in Z}.
 * https://oeis.org/A385557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(30, n) * (2 - (n mod 2))
 * Range:     u8 n <= 19, u16 n <= 1109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    // n is a positive integer (index starts at 1)
    seq_t g = gcd((seq_t)n, (seq_t)30);
    seq_t factor2 = ((n & 1) == 0) ? (seq_t)2 : (seq_t)1;
    return (seq_t)n * g * factor2;
}
