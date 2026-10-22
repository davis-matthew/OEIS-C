/*
 * A379936  E.g.f. A(x) satisfies A(x) = 1/( exp(-x*A(x)^(1/2)) - x )^2.
 * https://oeis.org/A379936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * sum_{k=0..n} (n+2)^(k-1) * binomial(n+2,n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* n! and 2*n! */
    seq_t fact_n   = factorial(n);
    seq_t two_fact = fact_n * (seq_t)2;

    /* k = 0 term: n! * (n+1)  */
    seq_t result = fact_n * (seq_t)(n + 1);

    /* Terms for k >= 1 */
    if (n >= 1) {
        seq_t factor = two_fact;               /* 2*n! / 1!  */
        seq_t bin    = binomial(n + 2, 3);      /* binom(n+2,3)  */
        seq_t power  = 1;                       /* (n+2)^(k-1) for k=1 */

        for (idx_t k = 1; k <= n; ++k) {
            /* term for current k */
            seq_t term = factor * bin * power;
            result += term;

            /* no more updates needed after the last term */
            if (k == n)
                break;

            /* prepare for k+1 */
            factor /= (seq_t)(k + 1);                 /* 2*n! / (k+1)!  */

            /* update binom(n+2, k+3) from binom(n+2, k+2) */
            seq_t tmp = bin * (seq_t)(n + 2 - (k + 2));
            bin = tmp / (seq_t)(k + 3);

            /* increase exponent by 1 */
            power *= (n + 2);
        }
    }

    return result;
}
