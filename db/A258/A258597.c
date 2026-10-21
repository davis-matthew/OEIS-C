/*
 * A258597  a(n) = 13*3^n.
 * https://oeis.org/A258597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13 * 3^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   mul_small, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t val = pow3(n);          /* val = 3^n   */
    val = mul_small(val, 13);     /* val = 13*3^n */
    return val;
}
