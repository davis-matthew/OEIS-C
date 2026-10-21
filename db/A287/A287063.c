/*
 * A287063  Number of dominating sets in the n-crown graph (for n > 1).
 * https://oeis.org/A287063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^n*(n + 2) + n^2 + n + 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;

    /* 4^n   = 2^(2n) */
    seq_t pow4 = pow2(2 * n);
    /* 2^n   */
    seq_t pow2n = pow2(n);

    /* Compute 4^n + n^2 + n + 3 first to avoid unsigned underflow */
    seq_t sum = pow4 + n_s * n_s + n_s + (seq_t)3;

    /* Subtract 2^n * (n + 2) */
    seq_t prod = pow2n * (n_s + 2);

    return sum - prod;
}
