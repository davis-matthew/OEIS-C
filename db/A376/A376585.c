/*
 * A376585  a(n) = 2^(2*n - 1) - 2^(n - 1)*(n - 1).
 * https://oeis.org/A376585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n - 1) - 2^(n - 1)*(n - 1) (for n ≥ 1) a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p2n      = pow2(n);          /* 2^n      */
    seq_t p2n_1    = pow2(n - 1);      /* 2^(n-1)  */
    seq_t term     = p2n - ((seq_t)n - 1); /* 2^n - (n-1) */
    return p2n_1 * term;               /* 2^(n-1) * (2^n - (n-1)) */
}
