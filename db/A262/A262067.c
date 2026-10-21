/*
 * A262067  a(n) = n^n - (n-2)^n.
 * https://oeis.org/A262067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - (n-2)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;        /* special case – 1^1 - (-1)^1 = 2 */

    /* pow_seq(base, exponent) : base is seq_t, exponent is int   */
    seq_t p1 = pow_seq((seq_t)n, (int)n);                 /* n^n */
    seq_t p2 = pow_seq((seq_t)(n - 2), (int)n);           /* (n-2)^n */
    return p1 - p2;
}
