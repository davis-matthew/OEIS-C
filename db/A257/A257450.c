/*
 * A257450  a(n) = 541*(2^n - 1) - 5*n^4 - 30*n^3 - 130*n^2 - 375*n.
 * https://oeis.org/A257450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 541*(2^n - 1) - 5*n^4 - 30*n^3 - 130*n^2 - 375*n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2_n = pow2(n);
    /* first part: 541*(2^n - 1) */
    seq_t term1 = 541 * (pow2_n - (seq_t)1);

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n2 * n2;       /* n^4 */

    /* second part: 5*n^4 + 30*n^3 + 130*n^2 + 375*n */
    seq_t term2 = 5 * n4 + 30 * n3 + 130 * n2 + 375 * nn;

    return term1 - term2;
}
