/*
 * A331352  Number of chiral pairs of colorings of the edges (or triangular
 *          faces) of a regular 4-dimensional simplex with n available colors.
 * https://oeis.org/A331352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*C(n,2) + 387*C(n,3) + 6320*C(n,4) + 41350*C(n,5) +
 *            135792*C(n,6) + 246540*C(n,7) + 252000*C(n,8) + 136080*C(n,9) +
 *            30240*C(n,10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"  /* Provides int binom(idx_t a, idx_t b); */

static const seq_t coeffs[9] = {
    (seq_t)6,
    (seq_t)387,
    (seq_t)6320,
    (seq_t)41350,
    (seq_t)135792,
    (seq_t)246540,
    (seq_t)252000,
    (seq_t)136080,
    (seq_t)30240
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 2; k <= 10 && k <= n; ++k) {
        seq_t c = binom(n, k);
        result += coeffs[k - 2] * c;
    }
    return result;
}
