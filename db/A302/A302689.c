/*
 * A302689  a(n) = 4 + 2^n - 4*n.
 * https://oeis.org/A302689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + 2^n - 4*n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    // Compute 2^n
    seq_t p = pow2(n);
    // 4 + 2^n
    seq_t result = p + (seq_t)4;
    // Subtract 4*n
    seq_t subtract = (seq_t)4 * (seq_t)n;
    result -= subtract;
    return result;
}
