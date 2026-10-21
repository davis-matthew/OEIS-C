/*
 * A268553  Diagonal of the rational function 1/((1 - u v - u w - v w) * (1 -
 *          x y - x z - y z)).
 * https://oeis.org/A268553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(3*n,n) * C(2*n,n))^2 * where C(m,k) = binomial
 *            coefficient
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Binomial(0,0) = 1, so the formula works for n = 0 as well. */
    seq_t comb3n = binomial(3 * n, n);   /* C(3n, n)   */
    seq_t comb2n = binomial(2 * n, n);   /* C(2n, n)   */

    seq_t prod = comb3n * comb2n;        /* product of the two binomials */
    return prod * prod;                  /* square of the product */
}
