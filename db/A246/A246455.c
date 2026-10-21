/*
 * A246455  G.f.: Sum_{n>=0} x^n / (1-2*x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          x^k] * [Sum_{k=0..n} C(n,k)^2 * 2^k * x^k].
 * https://oeis.org/A246455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} Sum_{j=0..n-2k} C(n-k, k+j)^2 * C(k+j,
 *            k)^2 * 2^j
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* outer sum over k */
    for (idx_t k = 0; k <= n / 2; ++k) {
        /* initial binomial coefficients for j = 0 */
        seq_t r = binom(n - k, k);     /* C(n-k, k)   */
        seq_t s = 1;                   /* C(k, k)     */

        idx_t maxj = n - 2 * k;
        seq_t pow2 = 1;                /* 2^j         */

        /* inner sum over j */
        for (idx_t j = 0; j <= maxj; ++j) {
            /* term = C(n-k, k+j)^2 * C(k+j, k)^2 * 2^j */
            seq_t term = r * r;       /* r^2         */
            term *= s * s;            /* * s^2       */
            term *= pow2;             /* * 2^j       */

            sum += term;

            if (j == maxj) break;    /* no further updates */

            /* advance r to C(n-k, k+j+1) */
            seq_t numerator_r = (seq_t)(n - 2 * k - j);
            seq_t denominator_r = (seq_t)(k + j + 1);
            r = (r * numerator_r) / denominator_r;

            /* advance s to C(k+j+1, k) */
            seq_t numerator_s = (seq_t)(k + j + 1);
            seq_t denominator_s = (seq_t)(j + 1);
            s = (s * numerator_s) / denominator_s;

            /* advance power of two */
            pow2 *= (seq_t)2;
        }
    }
    return sum;
}
