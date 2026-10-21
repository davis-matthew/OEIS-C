/*
 * A248338  a(n) = 10^n - 4^n.
 * https://oeis.org/A248338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t v10 = pow10(n);
    seq_t v4  = pow4(n);
    return v10 - v4;
}
