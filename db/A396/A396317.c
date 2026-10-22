/*
 * A396317  Number of n X n binary matrices with exactly 8 1's such that there
 *          are an even number of 1's in every row and column.
 * https://oeis.org/A396317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*C(n,4)^2 + 36*C(n,4)*C(n,3) + 2*C(n,2)*C(n,4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 21, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t nn = (seq_t)n;

    /* binomial coefficients using multiplicative formulas */
    seq_t comb2 = nn * (nn - 1) / 2;
    seq_t comb3 = nn * (nn - 1) * (nn - 2) / 6;
    seq_t comb4 = nn * (nn - 1) * (nn - 2) * (nn - 3) / 24;

    seq_t term1 = 90 * comb4 * comb4;          /* 90 * C(n,4)^2 */
    seq_t term2 = 36 * comb4 * comb3;          /* 36 * C(n,4) * C(n,3) */
    seq_t term3 = 2  * comb2 * comb4;          /* 2  * C(n,2) * C(n,4) */

    return term1 + term2 + term3;
}
