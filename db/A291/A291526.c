/*
 * A291526  a(n) = 2^n*(n - 3) + 4.
 * https://oeis.org/A291526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n - 3) + 4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    seq_t two_n = pow2(n);              /* 2^n */
    seq_t mult   = two_n * ((seq_t)(n - 3)); /* 2^n * (n-3) */
    return mult + (seq_t)4;              /* +4 */
}
