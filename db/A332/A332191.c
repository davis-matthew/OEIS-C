/*
 * A332191  a(n) = 10^(2*n+1) - 1 - 8*10^n.
 * https://oeis.org/A332191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n+1) - 1 - 8*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^(2n+1) */
    idx_t exp1 = (idx_t)2 * n + 1;
    seq_t pow1 = pow10(exp1);

    /* Compute 10^n */
    seq_t pow2 = pow10(n);

    /* a(n) = pow1 - 1 - 8*pow2 */
    return pow1 - (seq_t)1 - ((seq_t)8 * pow2);
}
