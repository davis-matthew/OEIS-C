/*
 * A256873  a(n) = 2^(n-1)*(2^n+5).
 * https://oeis.org/A256873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^n+5), a(0) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset term */
    if (n == 0) return (seq_t)3;

    /* Compute 2^n and 2^(n-1) */
    seq_t pow_n    = pow2(n);      /* 2^n   */
    seq_t pow_n_1  = pow2(n - 1);  /* 2^(n-1) */

    /* Compute the expression 2^n + 5 */
    seq_t term = pow_n + (seq_t)5;

    /* Return the final result */
    return pow_n_1 * term;
}
