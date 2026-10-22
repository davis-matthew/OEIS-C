/*
 * A380769  E.g.f. A(x) satisfies A(x) = exp(x / (1 - x*A(x)^2)) / (1 -
 *          x*A(x)^2).
 * https://oeis.org/A380769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..n} (2*n-2*k+1)^(k-1) * binomial(3*n-2*k,
 *            n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t fact_n = factorial(n);
    seq_t sum = 0;

    /* k = 0 term */
    seq_t bin0   = binomial(3*n, n);          /* binomial(3n, n)         */
    seq_t term0  = bin0 / (seq_t)(2*n + 1);   /* divides exactly          */
    sum += term0 * fact_n;                   /* multiply by n! outside  */

    /* loop for k = 1..n */
    seq_t B     = bin0;       /* current binomial(3n-2k, n-k) */
    seq_t nf_div = fact_n;    /* fact_n / k! (k! updated per iteration) */

    for (idx_t k = 1; k <= n; ++k) {
        /* update B to binomial(3n-2k, n-k) from B_{k-1} */
        seq_t m1 = (seq_t)(n - k + 1);
        seq_t m2 = (seq_t)(2 * n - k + 1);
        seq_t d1 = (seq_t)(3 * n - 2 * k + 2);
        seq_t d2 = (seq_t)(3 * n - 2 * k + 1);

        B = B * m1;
        B = B * m2;
        B = B / (d1 * d2);              /* exact quotient */

        /* update factorial division by k! */
        nf_div /= (seq_t)k;

        /* compute (2*n - 2*k + 1)^(k-1) */
        seq_t base    = (seq_t)(2 * n - 2 * k + 1);
        seq_t pow_val = big_pow(base, (seq_t)(k - 1));

        /* term = (n! / k!) * B * base^(k-1) */
        seq_t term = nf_div * B;
        term *= pow_val;
        sum += term;
    }

    return sum;
}
