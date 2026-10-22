/*
 * A338983  Number of chiral pairs of colorings of the 120 dodecahedral facets
 *          of the 4-D 120-cell (or 120 vertices of the 4-D 600-cell) using
 *          exactly n colors.
 * https://oeis.org/A338983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, easy
 * Formula:   a(n) = Sum_{j in {17,19,23,27,31,61,75}} n! * S2(j,n) / d_j *
 *            where d_j = 5, 5, 6, 6, 4, 120, 120. * Equivalent: a(n) =
 *            (1/120) * Sum_j (n! * S2(j,n) * (120/d_j)) * with j denoting the
 *            cycle–index parameters of the symmetry group. * S2(j,n) are
 *            Stirling subset numbers of the second kind. * For n...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Predefined parameters */
    static const idx_t J[7] = {17, 19, 23, 27, 31, 61, 75};
    static const seq_t W[7] = {(seq_t)24, (seq_t)24, (seq_t)20,
                               (seq_t)20, (seq_t)30, (seq_t)1, (seq_t)1};

    if (n > 75)
        return 0;

    /* One–time initialization of Stirling numbers and factorials */
    static bool init = false;
    static seq_t stirling[76][76];
    static seq_t fact[76];

    if (!init) {
        init = true;

        /* Stirling numbers of second kind */
        stirling[0][0] = 1;
        for (idx_t i = 1; i <= 75; ++i) {
            stirling[i][0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (seq_t)k * stirling[i - 1][k];
                seq_t term2 = stirling[i - 1][k - 1];
                stirling[i][k] = term1 + term2;
            }
        }

        /* Factorials */
        fact[0] = 1;
        for (idx_t i = 1; i <= 75; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
    }

    if (n == 0)
        return 0;

    seq_t result;
    if (n < 5) {
        /* Direct sum with common denominator 120 */
        seq_t num = 0;
        for (int idx = 0; idx < 7; ++idx) {
            seq_t term = fact[n] * stirling[J[idx]][n] * W[idx];
            num += term;
        }
        result = num / 120;   /* exact division */
    } else {
        /* Factor out n! / 120 and compute the integer sum */
        seq_t F = fact[n] / 120;   /* integer for n >= 5 */
        seq_t Wsum = 0;
        for (int idx = 0; idx < 7; ++idx)
            Wsum += stirling[J[idx]][n] * W[idx];
        result = F * Wsum;
    }
    return result;
}
