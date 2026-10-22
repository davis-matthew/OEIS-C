/*
 * A382032  E.g.f. A(x) satisfies A(x) = exp(x*C(x*A(x))^2), where C(x) = 1 +
 *          x*C(x)^2 is the g.f. of A000108.
 * https://oeis.org/A382032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} ( ∏_{t=0}^{k-1} (n-1-t) ) *
 *            (k+1)^{\,n-k-1} * C(2n,k) , a(0) = 1 where C(2n,k) =
 *            binomial(2n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t prod = 1;            /* ∏_{t=0}^{k-1} (n-1-t)   */
    seq_t binom = 1;           /* binomial(2n,k)          */
    const idx_t two_n = 2 * n;

    for (idx_t k = 0; k < n; ++k) {
        seq_t pow_val = pow_int((seq_t)(k + 1), n - k - 1);   /* (k+1)^{n-k-1} */
        seq_t term   = prod;
        term = term * binom;
        term = term * pow_val;
        sum = sum + term;

        /* Prepare values for the next k, if any */
        if (k + 1 < n) {
            /* Update product: multiply by (n-1-k) */
            prod = prod * (seq_t)(n - 1 - k);

            /* Update binomial: binom_{k+1} = binom_k * (2n - k) / (k+1)  */
            idx_t num = two_n - k;   /* 2n - k  */
            binom = (binom * (seq_t)num) / (seq_t)(k + 1);
        }
    }
    return sum;
}
