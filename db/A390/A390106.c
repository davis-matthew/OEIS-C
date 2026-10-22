/*
 * A390106  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-3*x/(1-x))
 *          ).
 * https://oeis.org/A390106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3 * n! * Σ_{k=1..n} (3*n+3)^(k-1) * binomial(n-1,k-1) / k!
 *
 * The implementation evaluates the sum by keeping the factor n!/k! as a
 * running product, thus avoiding factorials of all intermediate k and
 * keeping the arithmetic integer at every step.  All operations are
 * performed in the integer type seq_t supplied by the build.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (empty product) */
    if (n == 0)
        return 1;

    const seq_t N = (seq_t)n;               /* promote index to seq_t once */
    const seq_t base = (seq_t)(3 * N + 3);   /* 3*n + 3 */

    /* n! */
    seq_t prod = factorial(n);              /* prod holds n! / k! for current k */
    seq_t power = 1;                        /* (3*n+3)^(k-1) */
    seq_t binom = 1;                        /* binomial(n-1, k-1) */
    seq_t sum   = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = prod * power * binom
           prod = n! / k!  (current k) */
        sum += prod * power * binom;

        if (k < n)          /* prepare for next iteration */
        {
            /* prod_{k+1} = prod_k / (k+1)   (exact division) */
            prod /= (seq_t)(k + 1);

            /* power_{k+1} = power_k * (3*n+3) */
            power *= base;

            /* binom_{k+1} = binom_k * (n-k) / k
               (exact division because binomial coefficient) */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
        }
    }

    /* a(n) = 3 * sum */
    return sum * 3;
}
