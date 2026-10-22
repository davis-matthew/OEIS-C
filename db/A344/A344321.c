/*
 * A344321  a(n) = 2^(2*n - 5)*binomial(n-5/2, -1/2)*(36*n^4 - 78*n^3 + 54*n^2
 *          - 48*n + 24)/((n + 1)*n*(n - 1)) for n >= 2 and otherwise 1.
 * https://oeis.org/A344321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1, a(2)=8. For n>=3: a(n)=
 *            (3*n-2)*(1/n+1/2)*C(2*n-2,n-1) + 6*(n-2)*C(2*n-4,n-2) +
 *            (n-1)*(3*n-8)/(2*(2*n-3))*C(2*n-2,n-1) + 2*sum_{k=n-1}^{2*n-6}
 *            C(k,n-1)*(k+n+1) where C(m,r)=binomial(m,r)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    if (n == 2)
        return 8;

    /* n >= 3 */
    seq_t n_t = (seq_t)n;
    idx_t two_n_minus_2 = 2 * n - 2;
    idx_t n_minus_1 = n - 1;
    seq_t C = binomial(two_n_minus_2, n_minus_1);

    idx_t two_n_minus_4 = 2 * n - 4;
    idx_t n_minus_2 = n - 2;
    seq_t C2 = binomial(two_n_minus_4, n_minus_2);

    /* term1 = (3*n-2)*(1/n+1/2)*C = (3*n-2)*C*(n+2)/(2*n) */
    seq_t term1_num = (seq_t)(3 * n - 2) * C * (seq_t)(n + 2);
    seq_t term1_den = (seq_t)(2 * n);
    seq_t term1 = term1_num / term1_den;

    /* term2 = 6*(n-2)*C2 */
    seq_t term2 = (seq_t)6 * (n - 2) * C2;

    /* term3 = (n-1)*(3*n-8)/(2*(2*n-3))*C */
    seq_t term3_num = (seq_t)(n - 1) * (seq_t)(3 * n - 8) * C;
    seq_t term3_den = (seq_t)2 * (seq_t)(2 * n - 3);
    seq_t term3 = term3_num / term3_den;

    /* term4 = 2 * sum_{k=n-1}^{2*n-6} binomial(k,n-1)*(k + n + 1) */
    seq_t sum = 0;
    /* binom_k = C(k, n-1), start with k = n-1: binom = 1 */
    seq_t binom_k = 1;
    idx_t k_start = n - 1;
    idx_t k_end = 2 * n - 6;

    for (idx_t k = k_start; k <= k_end; ++k) {
        /* add current term */
        sum += binom_k * (seq_t)(k + n + 1);

        /* update binomial for next k */
        if (k < k_end) {
            idx_t next_k = k + 1;
            binom_k = binom_k * (seq_t)next_k /
                      (seq_t)(next_k - (n - 1));
        }
    }
    seq_t term4 = sum * 2;

    return term1 + term2 + term3 + term4;
}
