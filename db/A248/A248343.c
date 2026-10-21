/*
 * A248343  a(n) = 10^n - 8^n.
 * https://oeis.org/A248343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 handled by pow_uint itself: 10^0 = 1, 8^0 = 1 */
    seq_t ten_pow  = pow_uint(10ULL, n);
    seq_t eight_pow = pow_uint(8ULL, n);
    return ten_pow - eight_pow;
}
