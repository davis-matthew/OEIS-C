/*
 * A390216  E.g.f. A(x) satisfies A(x) = exp(x/(1+x^3)^2 * A(x)).
 * https://oeis.org/A390216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k * * (n-3k+1)^(n-3k-1) *
 *            binomial(2*n-5*k-1, k) / (n-3k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    idx_t kmax = n / 3;

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 3 * k;              /* m = n - 3k >= 0 */
        seq_t term = 0;

        /* binomial part – may be zero for some k */
        seq_t binom_val = binomial(2 * n - 5 * k - 1, k);
        if (binom_val == 0)
            continue;                     /* contributes nothing */

        /* power part: (m+1)^(m-1), defined for m>=1 */
        seq_t pow_term;
        if (m >= 1) {
            pow_term = big_pow((seq_t)(m + 1), (seq_t)(m - 1));
        } else {
            /* for m==0 the exponent is -1 but the binomial is zero,
               so this path will not be used. Use 1 as neutral element. */
            pow_term = 1;
        }

        /* ratio n! / m! = product from m+1 to n */
        seq_t ratio = 1;
        for (idx_t i = m + 1; i <= n; ++i) {
            ratio *= (seq_t)i;
        }

        /* term = (m+1)^(m-1) * binom * ratio */
        term = pow_term;
        term *= binom_val;
        term *= ratio;

        /* accumulate with appropriate sign */
        if (k & 1) {
            neg_sum += term;
        } else {
            pos_sum += term;
        }
    }

    /* pos_sum should be >= neg_sum for all n>=1 */
    return pos_sum - neg_sum;
}
