/*
 * A304606  a(n) = 54*2^n + 28 (n >= 1).
 * https://oeis.org/A304606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*2^n + 28
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n >= 1, compute 54 * 2^n + 28 */
    seq_t result = pow2(n);               /* 2ⁿ */
    result = result * (seq_t)54 + (seq_t)28;
    return result;
}
