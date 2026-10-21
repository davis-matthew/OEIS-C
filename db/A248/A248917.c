/*
 * A248917  a(n) = 2^n * n^2 + 1.
 * https://oeis.org/A248917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);
    seq_t nsq   = (seq_t)n * (seq_t)n;
    seq_t res   = pow2n * nsq + 1;
    return res;
}
