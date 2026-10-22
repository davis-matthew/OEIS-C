/*
 * A385556  Period of {binomial(N,4) mod n: N in Z}.
 * https://oeis.org/A385556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(6, n) * (2 - (n mod 2))
 * Range:     u8 n <= 23, u16 n <= 5465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is at least 1 */
    seq_t gn6   = gcd((seq_t)6, (seq_t)n);           /* gcd(6, n) */
    seq_t parity = (seq_t)(2 - (n & 1));             /* 2 - (n mod 2) */
    return (seq_t)n * gn6 * parity;
}
