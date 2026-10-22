/*
 * A387429  Self-convolution of n-th antidiagonal of the natural number array
 *          (so named in the Comments section of A000027).
 * https://oeis.org/A387429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4 + 5*n^2 + 3*n^4)/12 (OEIS A387429)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defensive for n < 1 */

    seq_t sn   = (seq_t)n;                 /* n as seq_t                     */
    seq_t sn2  = sn * sn;                  /* n^2                            */
    seq_t sn4  = sn2 * sn2;                /* n^4                            */
    seq_t term = (seq_t)4 + (seq_t)5 * sn2 + (seq_t)3 * sn4; /* 4 + 5n^2 + 3n^4 */
    return sn * term / (seq_t)12;          /* final formula                 */
}
