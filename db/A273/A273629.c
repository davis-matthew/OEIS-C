/*
 * A273629  a(n) = (9*n)!/((7*n)!*n!^2).
 * https://oeis.org/A273629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(9*n,2*n) * binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    seq_t v1 = binomial(9*n, 2*n);
    seq_t v2 = binomial(2*n, n);
    return v1 * v2;
}
