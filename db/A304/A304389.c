/*
 * A304389  a(n) = 126*2^n - 22 (n>=1).
 * https://oeis.org/A304389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 126*2^n - 22 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);           /* 2^n */
    v *= (seq_t)126;             /* 126 * 2^n */
    v -= (seq_t)22;              /* subtract 22 */
    return v;
}
