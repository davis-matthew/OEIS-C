/*
 * A389843  E.g.f. A(x) satisfies A(x) = exp(x^2 * A(x) / (1-x)^2).
 * https://oeis.org/A389843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..⌊n/2⌋} (k+1)^(k-1) * C(n-1, n-2k) / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An O(n log n) algorithm. It uses a recurrence for the binomial
   coefficients to keep the inner loop linear. */
#include <stdint.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Compute n! once. */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t total = 0;

    idx_t max_k = n / 2;
    if (max_k == 0)   /* only possible when n == 1 */
        return total;

    /* binom_k holds C(n-1, n-2k) for current k */
    seq_t binom_k = (seq_t)(n - 1);      /* k = 1 => C(n-1, n-2) = n-1 */

    /* factorial_k holds k! for current k */
    seq_t fact_k = 1;                    /* 1! = 1 */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        if (k > 1)
        {
            /* Update binomial using recurrence:
               C(m, r-2) = C(m, r) * r * (r-1) /
                               ((m-r+1) * (m-r+2))
               Here m = n-1, r = n-2k+2. */
            idx_t r_prev = n - 2 * k + 2;               /* r_{k-1} */
            seq_t num = (seq_t)(r_prev) * (seq_t)(r_prev - 1);
            seq_t den = (seq_t)(2 * k - 2) * (seq_t)(2 * k - 1);
            binom_k = binom_k * num / den;
        }

        /* (k+1)^(k-1) */
        seq_t pow_val = big_pow((seq_t)(k + 1), (seq_t)(k - 1));

        /* term = n! * binom_k / k! * pow_val */
        seq_t term = fact_n * binom_k;
        term = term / fact_k;
        term = term * pow_val;

        total += term;

        /* Prepare factorial_k for next iteration: (k+1)! */
        fact_k *= (seq_t)(k + 1);
    }

    return total;
}
