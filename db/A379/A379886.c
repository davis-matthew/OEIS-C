/*
 * A379886  E.g.f. A(x) satisfies A(x) = 1/(exp(-x) - x*A(x)^2).
 * https://oeis.org/A379886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A379886
   Formula: a(n) = n! * Σ_{k=0..n} (3*n-3*k+1)^(k-1) * C(3*n-3*k+1, n-k) / k!
   The implementation computes the sum using integer arithmetic only.
   For each k we replace n!/k! by (n! / k!) and handle the t^(k-1) term
   (t = 3*n-3*k+1).  When k = 0, the t^(k-1) factor becomes 1/t, which
   division is exact (C(t,n) is divisible by t).  Intermediate results
   are kept in the seq_t type and the product is accumulated in `sum`.
   At the end `sum` already equals a(n).
*/

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special case n == 0: a(0) = 1, handled by the generic loop below. */
    /* Compute n! once. */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= i;

    seq_t sum = 0;
    seq_t fact_k = 1; /* k! for current k (starts with 0! = 1). */

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t t   = 3 * n - 3 * k + 1;          /* t = 3*n - 3*k + 1   */
        seq_t bin = binomial(t, n - k);         /* C(t, n-k)           */
        seq_t ratio = fact_n / fact_k;          /* n! / k!             */
        seq_t term;

        if (k == 0)
        {
            /* term = (n! * bin) / t   (exact division) */
            term = (ratio * bin) / (seq_t)t;
        }
        else
        {
            /* term = (n! / k!) * t^(k-1) * bin   */
            seq_t tpow = seq_pow((seq_t)t, k - 1);
            term = ratio * tpow * bin;
        }

        sum += term;

        /* Update fact_k to (k+1)! for next iteration. */
        fact_k *= (k + 1);
    }

    return sum;
}
