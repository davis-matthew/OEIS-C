/*
 * A321002  a(0)=3; thereafter a(n) = 20*6^(n-1)-2^(n-1).
 * https://oeis.org/A321002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3; a(n)=20*6^(n-1)-2^(n-1) for n≥1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3;
    }

    /* Compute 6^(n-1) and 2^(n-1).  seq_pow works for any seq_t. */
    seq_t pow6 = seq_pow((seq_t)6, n - 1);
    seq_t pow2 = seq_pow((seq_t)2, n - 1);

    /* 20 * 6^(n-1) - 2^(n-1) */
    seq_t result = pow6 * (seq_t)20;
    result -= pow2;

    return result;
}
