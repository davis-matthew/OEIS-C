/*
 * A321421  a(n) = 10*(4^n - 1)/3 + 1.
 * https://oeis.org/A321421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*(4^n - 1)/3 + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t val = pow4(n);   /* 4^n */
    val -= (seq_t)1;       /* 4^n - 1 */
    val /= (seq_t)3;       /* (4^n - 1)/3   (divisible by 3) */
    val *= (seq_t)10;      /* *10 */
    val += (seq_t)1;       /* +1 */
    return val;
}
