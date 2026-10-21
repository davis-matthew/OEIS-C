/*
 * A257449  a(n) = 75*(2^n - 1) - 4*n^3 - 18*n^2 - 52*n.
 * https://oeis.org/A257449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 75*(2^n - 1) - 4*n^3 - 18*n^2 - 52*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 75 * (2^n - 1) */
    seq_t val = pow2(n) * (seq_t)75U;
    val -= (seq_t)75U;

    /* subtract 4*n^3 + 18*n^2 + 52*n */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t sub = (n3 * (seq_t)4U) + (n2 * (seq_t)18U) + (ns * (seq_t)52U);

    val -= sub;
    return val;
}
