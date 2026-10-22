/*
 * A324568  a(n) = Sum_{i=0..n, j=0..n} (binomial(2*i, j) + binomial(2*j, i)).
 * https://oeis.org/A324568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n, j=0..n} (binomial(2*i, j) + binomial(2*j,
 *            i))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

#define DIRECT_LIMIT 31  /* use direct summation for n <= 31 (fits in uint64_t) */

seq_t a(idx_t n) {
    /* ----- small n: compute directly ------------------------------------- */
    if (n <= DIRECT_LIMIT) {
        seq_t sum = 0;
        idx_t i, j;
        for (i = 0; i <= n; ++i) {
            idx_t two_i = 2 * i;
            for (j = 0; j <= n; ++j) {
                /* binom(2*i, j) + binom(2*j, i) */
                sum += binom(two_i, j) + binom(2 * j, i);
            }
        }
        return sum;
    }

    /* ----- large n: use the 4‑term recurrence ---------------------------- */
    seq_t prev[4] = {124, 32, 8, 2};  /* a(3), a(2), a(1), a(0) */

    idx_t k;
    for (k = 4; k <= n; ++k) {
        seq_t nn = (seq_t)k;
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;

        /* coefficients (all positive except the second term) */
        seq_t coeff1 = (seq_t)85 * n3 - (seq_t)332 * n2 + (seq_t)243 * nn + (seq_t)100;
        seq_t coeff2 = (seq_t)65 * n3 - (seq_t)298 * n2 + (seq_t)377 * nn - (seq_t)100;
        seq_t coeff3 = (seq_t)20 * n3 - (seq_t)109 * n2 + (seq_t)191 * nn - (seq_t)100;
        seq_t coeff4 = (seq_t)8 * ((seq_t)2 * nn - (seq_t)5) *
                       ((seq_t)5 * n2 - (seq_t)11 * nn + (seq_t)4);

        /* terms of the numerator */
        seq_t term1 = coeff1 * prev[0];
        seq_t term2 = coeff2 * prev[1] * (seq_t)3;   /* will be subtracted */
        seq_t term3 = coeff3 * prev[2] * (seq_t)2;
        seq_t term4 = coeff4 * prev[3];

        seq_t numerator = term1 + term3 + term4;
        numerator -= term2;   /* term2 is subtracted */

        /* denominator */
        seq_t denom = (seq_t)2 * (nn + (seq_t)1) *
                      ((seq_t)5 * n2 - (seq_t)21 * nn + (seq_t)20);

        seq_t next = numerator / denom;

        /* shift the previous values */
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = next;
    }

    return prev[0];
}
