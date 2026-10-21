/*
 * A281482  a(n) = 2^(n + 1) * (2^n + 1) - 1.
 * https://oeis.org/A281482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) * (2^n + 1) - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 2^(n+1) using the provided pow2 function */
    seq_t two_n   = pow2(n);
    seq_t two_n1 = pow2(n + 1);

    /* a(n) = 2^(n+1) * (2^n + 1) - 1 */
    seq_t result = two_n1 * (two_n + 1) - 1;
    return result;
}
