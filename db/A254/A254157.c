/*
 * A254157  a(n) = binomial(3*n,n)^n.
 * https://oeis.org/A254157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n,n)^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial with 3n on one side */
    seq_t b = binomial((idx_t)(3 * n), n);
    /* Raise the result to the power n */
    return seq_pow(b, n);
}
