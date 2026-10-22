/*
 * A364939  E.g.f. satisfies A(x) = exp( x*A(x) / (1 - x*A(x))^2 ).
 * https://oeis.org/A364939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364939
   Formula: a(n) = n! * Σ_{k=0..n} (n+1)^(k-1) * binomial(n+k-1, n-k) / k!
   (the k=0 term vanishes for n≥1) */
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* n! */
    seq_t fact_n = factorial(n);

    seq_t sum = 0;
    seq_t mult = fact_n;          /* n! / 1! for k = 1 */
    seq_t pow_term = 1;           /* (n+1)^(k-1) with k = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial coefficient C(n+k-1, n-k) */
        seq_t bin = binomial((idx_t)(n + k - 1), (idx_t)(n - k));

        /* term = (n! / k!) * (n+1)^(k-1) * bin */
        seq_t term = mult;
        term *= pow_term;
        term *= bin;
        sum += term;

        /* prepare for next k */
        if (k < n)
        {
            mult /= (seq_t)(k + 1);          /* mult = n! / (k+1)! */
            pow_term *= (seq_t)(n + 1);      /* next power of (n+1) */
        }
    }

    return sum;
}
