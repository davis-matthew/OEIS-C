/*
 * A291537  a(n) = 8^n - 3*2^n + 5.
 * https://oeis.org/A291537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 3*2^n + 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 8^n as 2^(3n) and 2^n using pow2 */
    seq_t pow8  = pow2((idx_t)3 * n);
    seq_t pow2n = pow2(n);

    /* a(n) = 8^n - 3*2^n + 5 */
    return pow8 - (seq_t)3 * pow2n + (seq_t)5;
}
