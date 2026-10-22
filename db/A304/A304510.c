/*
 * A304510  a(n) = 69*2^n - 42 (n>=1).
 * https://oeis.org/A304510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 69*2^n - 42 (n>=1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
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
    /* multiply by 69 and subtract 42 */
    val = val * (seq_t)69;
    val = val - (seq_t)42;
    return val;
}
