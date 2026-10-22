/*
 * A304514  a(n) = 33*2^n - 45 (n>=1).
 * https://oeis.org/A304514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*2^n - 45 (n >= 1, offset 1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = 33*2^n - 45 */
    seq_t power = pow2(n);                /* 2^n   */
    seq_t result = power * (seq_t)33;     /* 33*2^n*/
    result = result - (seq_t)45;          /* subtract 45 */
    return result;
}
