/*
 * A284850  a(n) = 4^n - 3^n - n.
 * https://oeis.org/A284850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n - n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t val = pow4(n);          /* 4^n */
    val -= pow3(n);               /* subtract 3^n */
    val -= (seq_t)n;              /* subtract n */
    return val;
}
