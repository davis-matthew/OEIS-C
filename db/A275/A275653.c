/*
 * A275653  a(n) = binomial(4*n,2*n)*binomial(3*n,2*n).
 * https://oeis.org/A275653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n,2*n) * binomial(3*n,2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b1 = binomial(4 * n, 2 * n);   /* binomial(4n,2n) */
    seq_t b2 = binomial(3 * n, 2 * n);   /* binomial(3n,2n) */
    return b1 * b2;
}
