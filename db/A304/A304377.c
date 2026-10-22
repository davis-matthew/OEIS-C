/*
 * A304377  a(n) = 102*2^n - 96 (n>=1).
 * https://oeis.org/A304377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*2^n - 96
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t val = pow2(n);
    /* a(n) = 102 * 2^n - 96 */
    val = (seq_t)102 * val - (seq_t)96;
    return val;
}
