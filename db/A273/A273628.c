/*
 * A273628  a(n) = (7*n)!/((5*n)!*n!^2).
 * https://oeis.org/A273628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n,2*n)*binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t seven_n = 7 * n;
    idx_t two_n   = 2 * n;
    seq_t c1 = binom(seven_n, two_n);
    seq_t c2 = binom(two_n, n);
    return c1 * c2;
}
