/*
 * A341718  Subtract 1 from each term of A004094 (the powers of 2 written
 *          backwards).
 * https://oeis.org/A341718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse(2^n) - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t val = pow_uint((uint64_t)2, n);

    /* Reverse its decimal digits */
    seq_t rev = 0;
    seq_t temp = val;
    const seq_t ten = (seq_t)10;

    while (temp > 0)
    {
        seq_t d = temp % ten;
        rev = rev * ten + d;
        temp = temp / ten;
    }

    /* Subtract 1 */
    return rev - (seq_t)1;
}
