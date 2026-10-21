/*
 * A296198  Harary index of the n X n black bishop graph.
 * https://oeis.org/A296198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-15 + 20*n - 30*n^2 + 16*n^3 + 6*n^4 - 3*(-1)^n*(2*n^2 +
 *            4*n - 5)) / 96 The sequence starts at n = 1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    long long nn = (long long)n;

    /* polynomial part */
    long long term = -15
                     + 20 * nn
                     - 30 * nn * nn
                     + 16 * nn * nn * nn
                     + 6  * nn * nn * nn * nn;

    /* parity-dependent part */
    long long par = 2 * nn * nn + 4 * nn - 5;
    if ((nn & 1) == 0)           /* n even: (-1)^n = +1  */
        term -= 3 * par;
    else                          /* n odd : (-1)^n = -1  */
        term += 3 * par;

    return (seq_t)(term / 96);
}
