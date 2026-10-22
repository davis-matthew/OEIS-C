/*
 * A346295  a(n) = Sum_{k=0..n} (2^k + 1) * (2^k + 2) / 2.
 * https://oeis.org/A346295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1) + 4) * (2^(n+1) + 5) / 6 - 4 + n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pw  = pow2(n + 1);          /* 2^(n+1) */
    seq_t t1  = pw + (seq_t)4;
    seq_t t2  = pw + (seq_t)5;
    seq_t prod = t1 * t2;
    seq_t result = prod / (seq_t)6;
    result -= (seq_t)4;
    result += (seq_t)n;
    return result;
}
