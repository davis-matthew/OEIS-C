/*
 * A241888  a(n) = 2^(4*n + 1) - 1.
 * https://oeis.org/A241888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n + 1) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 4*n + 1. */
    idx_t exp = 4 * n + 1;

    /* 2^exp as a seq_t. */
    seq_t val = pow2(exp);

    /* Subtract 1 to get a(n). */
    return val - 1;
}
