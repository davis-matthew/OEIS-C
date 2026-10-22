/*
 * A304515  a(n) = 159*2^n - 222 (n>=1).
 * https://oeis.org/A304515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 159*2^n - 222
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);          /* p = 2^n  */
    seq_t r = p * (seq_t)159;   /* 159*2^n */
    r -= (seq_t)222;            /* subtract 222 */
    return r;
}
