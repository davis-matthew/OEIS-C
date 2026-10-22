/*
 * A382029  E.g.f. A(x) satisfies A(x) = exp(x*C(x*A(x)^2)), where C(x) = 1 +
 *          x*C(x)^2 is the g.f. of A000108.
 * https://oeis.org/A382029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n-1} (2k+1)^(n-k-1) * binomial(n+k, k) / *
 *            ((n+k)(n-k-1)!) , a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    /* prefactor = n! / (n-k-1)! */
    seq_t prefactor = (seq_t)n;

    for (idx_t k = 0; k < n; ++k) {
        idx_t exp = n - k - 1;               /* n-k-1 */
        seq_t base = (seq_t)(2 * k + 1);      /* 2k+1 */

        /* (2k+1)^(n-k-1) */
        seq_t p = big_pow(base, (seq_t)exp);

        /* binomial(n+k, k) */
        seq_t bin = binomial(n + k, k);

        /* numerator = bin * p * prefactor */
        seq_t numer = bin;
        numer *= p;
        numer *= prefactor;

        /* divide by (n+k) */
        seq_t term = numer / (seq_t)(n + k);

        sum += term;

        /* update prefactor for next k:
           prefactor_{k+1} = prefactor_k * (n-k-1)
        */
        if (k < n - 1) /* avoid multiplication by zero when k=n-1 */
            prefactor *= (seq_t)(n - k - 1);
    }

    return sum;
}
