/*
 * A304388  a(n) = 144*2^n - 20 (n>=1).
 * https://oeis.org/A304388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*2^n - 20, Offset: 1
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t v = pow2(n);    /* v = 2^n */
    v *= (seq_t)144;      /* v = 144 * 2^n */
    v -= (seq_t)20;       /* v = 144*2^n - 20 */
    return v;
}
