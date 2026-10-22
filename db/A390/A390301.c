/*
 * A390301  E.g.f. A(x) satisfies A(x) = exp( x/(1-x^2) * A(x)^2 ).
 * https://oeis.org/A390301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} (2*(n-2k)+1)^(n-2k-1) *
 *              binomial(n-k-1, k) / (n-2k)!
 *
 * The implementation follows this formula directly.  For each k we
 * maintain the ratio n!/m! where m = n-2k, incrementally updated as
 * prod *= m*(m-1).  The term for k=0 has prod = 1 (n!/n! = 1).
 * The sum is accumulated in a seq_t variable.  All large integer
 * operations use the helper functions `binomial` and `big_pow`
 * provided by the build system.
 *
 * This function does NOT use memoisation; it recomputes each
 * value independently, which satisfies the requirement that calls can
 * arrive in arbitrary order.  The algorithm runs in O(n) iterations
 * with fast exponentiation for each term.  The helper functions
 * perform arithmetic in arbitrary precision when needed.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* base case a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* term summation */
    seq_t sum = (seq_t)0;
    seq_t prod = (seq_t)1;          /* will hold n! / m!  */

    idx_t k_max = n >> 1;           /* floor(n/2)          */
    idx_t m = n;                    /* m = n - 2*k         */

    for (idx_t k = 0; k <= k_max; ++k)
    {
        /* skip the k where m==0 because binomial will be zero
           and exponent would be negative.  Term contributes zero. */
        if (m == 0)
            break;

        /* binomial coefficient: C(n-k-1, k) */
        seq_t bin = binomial(n - k - 1, k);

        /* power term: (2*m+1)^(m-1) */
        seq_t pow_term = big_pow((seq_t)(2 * m + 1), (seq_t)(m - 1));

        /* term = prod * bin * pow_term */
        seq_t term = prod;
        term *= bin;
        term *= pow_term;

        sum += term;

        /* prepare for next k:
           prod_next = prod * m * (m-1)   (since n!/(m-2)! = n!/m! * m*(m-1))
           m decreases by 2.               */
        if (k < k_max)                /* no need to update after last term */
        {
            prod *= (seq_t)m;
            prod *= (seq_t)(m - 1);
            m -= 2;
        }
    }

    return sum;
}
