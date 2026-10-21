/*
 * A277924  a(n) = Sum_{i=0..n+1} binomial(2*n,n-i+1).
 * https://oeis.org/A277924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3*n+1)*Catalan(n)+4^n)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Directly handle the offset term a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Catalan number C_n */
    seq_t catalan_val = catalan(n);

    /* Coefficient 3*n + 1 */
    seq_t coeff = (seq_t)(3*n + 1);

    /* Compute (coeff * catalan_val) / 2 in a way that minimizes overflow
       by dividing one factor by 2 before the multiplication when possible. */
    seq_t part1;
    if ((coeff & 1) == 0) /* coeff is even */
        part1 = catalan_val * (coeff >> 1);
    else                 /* coeff is odd */
        part1 = (catalan_val * coeff) >> 1;

    /* 4^n / 2 = 2^(2*n-1) */
    seq_t pow2_half = pow2((idx_t)(2*n - 1));

    /* Final result */
    return part1 + pow2_half;
}
