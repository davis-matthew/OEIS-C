/*
 * A372844  a(n) is the number of parking functions of order n for which the
 *          fourth spot is lucky.
 * https://oeis.org/A372844
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5/8)*(n+1)^(n-1) - (1/8)*(13*n^2 - 26*n + 9)*(n-3)^(n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 4. For n < 4 we return 0. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Compute (n+1)^(n-1). */
    seq_t base1 = (seq_t)(n + 1);
    seq_t exp1  = (seq_t)(n - 1);
    seq_t pow1  = big_pow(base1, exp1);

    /* term1 = 5 * pow1 / 8 */
    seq_t term1 = ((seq_t)5 * pow1) / (seq_t)8;

    /* Compute (n-3)^(n-3). */
    seq_t base2 = (seq_t)(n - 3);
    seq_t exp2  = (seq_t)(n - 3);
    seq_t pow2  = big_pow(base2, exp2);

    /* Compute the quadratic coefficient: 13*n^2 - 26*n + 9 */
    seq_t coeff = ((seq_t)13 * (seq_t)n * (seq_t)n)
                  - ((seq_t)26 * (seq_t)n)
                  + (seq_t)9;

    /* term2 = coeff * pow2 / 8 */
    seq_t term2 = (coeff * pow2) / (seq_t)8;

    return term1 - term2;
}
