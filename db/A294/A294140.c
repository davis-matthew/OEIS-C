/*
 * A294140  Number of total dominating sets in the n-crown graph.
 * https://oeis.org/A294140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1 - n)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Optional: sequence is defined for n ≥ 1 */
    seq_t two_pow_n = pow_uint(2, n);     /* compute 2^n */
    seq_t temp = two_pow_n - (seq_t)(1 + n); /* subtract 1 and n */
    return temp * temp;                  /* square the result */
}
