/*
 * A336181  a(n) = Sum_{k=0..n} (-2)^k * binomial(n,k)^3.
 * https://oeis.org/A336181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2)^k * binomial(n,k)^3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t bin = 1;          /* binomial(n,0) = 1 */
    seq_t pow_neg2 = 1;     /* (-2)^0 = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = (-2)^k * binomial(n,k)^3 */
        seq_t term = bin * bin * bin;      /* bin^3 */
        term = term * pow_neg2;            /* *(-2)^k */
        sum += term;

        /* Prepare for next k */
        if (k < n) {
            /* binom(n,k+1) = binom(n,k) * (n-k)/(k+1) */
            bin = bin * (seq_t)(n - k);
            bin = bin / (seq_t)(k + 1);
            pow_neg2 = pow_neg2 * (seq_t)(-2);
        }
    }
    return sum;
}
