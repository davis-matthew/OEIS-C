/*
 * A329625  Smallest BII-number of a connected set-system with n edges.
 * https://oeis.org/A329625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^n - 1 for all n that are not powers of two; if n is a
 *            power of two and n > 1, then a(n) = 2^n + 2^(n-1) - 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_n = pow2(n);
    if (n > 1 && (n & (n - 1)) == 0) { /* n is a power of two */
        seq_t pow_n_minus1 = pow2(n - 1);
        return pow_n + pow_n_minus1 - (seq_t)1;
    }
    return pow_n - (seq_t)1;
}
