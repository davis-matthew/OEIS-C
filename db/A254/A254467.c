/*
 * A254467  a(n) = 15*4^n + 70*2^n + 35*3^n + 5^(n+1) + 6^n + 126.
 * https://oeis.org/A254467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*4^n + 70*2^n + 35*3^n + 5^(n+1) + 6^n + 126
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow2 = pow_uint(2, n);          /* 2^n */
    seq_t pow3 = pow_uint(3, n);          /* 3^n */
    seq_t pow4 = pow_uint(4, n);          /* 4^n */
    seq_t pow5 = pow_uint(5, n + 1);      /* 5^(n+1) */
    seq_t pow6 = pow_uint(6, n);          /* 6^n */

    seq_t result = ((seq_t)15) * pow4
                 + ((seq_t)70) * pow2
                 + ((seq_t)35) * pow3
                 + pow5
                 + pow6
                 + (seq_t)126;
    return result;
}
