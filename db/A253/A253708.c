/*
 * A253708  Numbers c(n) whose squares are equal to the sums of consecutive
 *          cubed integers b^3 + (b+1)^3 + ... + (b+M-1)^3 = c^2, for a first
 *          term b(n) being an odd squared integer (A016754).
 * https://oeis.org/A253708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/2) * ( 4*(2n+1)^4 – 1 )
 * Range:     u8 none, u16 n <= 3, u32 n <= 22, u64 n <= 911, u128 n <= 9459,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* term1 = n*(n+1)/2 */
    seq_t term1 = nn * (nn + 1) / 2;

    /* two_n_plus_one = 2*n + 1 */
    seq_t t = nn * 2 + 1;

    /* t^2 and t^4 */
    seq_t t2 = t * t;
    seq_t t4 = t2 * t2;

    /* term2 = 4*t^4 - 1 */
    seq_t term2 = 4 * t4 - 1;

    /* a(n) = term1 * term2 */
    return term1 * term2;
}
