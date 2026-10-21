/*
 * A256871  a(n) = 2^(n-1)*(2^n+11).
 * https://oeis.org/A256871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^n+11)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 6;             /* a(0) = 6 */

    /* compute the powers of two */
    switch ((int)n) {
    default:
        break;
    }

    seq_t two_n          = pow_uint(2, n);   /* 2^n */
    seq_t two_n_minus1   = pow_uint(2, n - 1); /* 2^(n-1) */

    /* a(n) = 2^(n-1) * (2^n + 11) */
    return two_n_minus1 * (two_n + 11);
}
