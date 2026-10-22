/*
 * A338964  Number of oriented colorings of the 120 dodecahedral facets of the
 *          4-D 120-cell (or 120 vertices of the 4-D 600-cell) using subsets
 *          of a set of n colors.
 * https://oeis.org/A338964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (960*n^4 + 1440*n^6 + 960*n^8 + 1200*n^10 + 336*n^12 +
 *            288*n^16 * + 40*n^20 + 400*n^22 + 336*n^24 + 60*n^30 + 288*n^32
 *            + 40*n^40 * + 400*n^44 + n^60 + 450*n^62 + n^120) / 7200
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t N = (seq_t)n;
    /* Compute powers up to 120 */
    seq_t powers[121];
    powers[0] = (seq_t)1;
    for (size_t i = 1; i <= 120; ++i)
        powers[i] = powers[i-1] * N;

    /* Define coefficients and exponents */
    static const unsigned int coeffs[] = {
        960, 1440, 960, 1200, 336, 288,
        40, 400, 336, 60, 288, 40,
        400, 1, 450, 1
    };
    static const unsigned int exps[] = {
        4, 6, 8, 10, 12, 16,
        20, 22, 24, 30, 32, 40,
        44, 60, 62, 120
    };
    const size_t terms = sizeof(coeffs) / sizeof(coeffs[0]);

    /* Sum up all terms */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < terms; ++i) {
        seq_t term = powers[exps[i]] * (seq_t)coeffs[i];
        sum += term;
    }

    /* Divide by 7200 (exact division) */
    const seq_t div = (seq_t)7200;
    return sum / div;
}
