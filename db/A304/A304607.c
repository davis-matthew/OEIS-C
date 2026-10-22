/*
 * A304607  a(n) = 252*2^n + 140 (n>=1).
 * https://oeis.org/A304607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 252*2^n + 140
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t value */
    seq_t p = pow2(n);

    /* Multiply by 252 and add 140 */
    return ((seq_t)252) * p + (seq_t)140;
}
