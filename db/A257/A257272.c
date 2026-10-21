/*
 * A257272  a(n) = 2^(n-1)*(2^n+7).
 * https://oeis.org/A257272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^n+7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    seq_t pow_n = pow2(n);
    seq_t pow_n_minus1 = pow2(n - 1);
    seq_t sum = pow_n + (seq_t)7;
    return pow_n_minus1 * sum;
}
