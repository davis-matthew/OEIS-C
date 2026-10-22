/*
 * A355142  a(n) = 33648*3^n - 1.
 * https://oeis.org/A355142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33648*3^n - 1
 * Range:     u8 none, u16 n <= 0, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t val = seq_pow((seq_t)3, n);   /* compute 3^n */
    val = mul_small(val, 33648);        /* multiply by 33648 */
    val = val - 1;                     /* subtract 1 */
    return val;
}
