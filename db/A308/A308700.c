/*
 * A308700  a(n) = n * 2^(n - 2) * (2^(n - 1) - 1).
 * https://oeis.org/A308700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * 2^(n-2) * (2^(n-1) - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t pow_n_minus_2 = pow2(n - 2);
    seq_t pow_n_minus_1 = pow2(n - 1);

    return n_seq * pow_n_minus_2 * (pow_n_minus_1 - 1);
}
