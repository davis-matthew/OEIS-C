/*
 * A370876  Expansion of e.g.f. (1/x) * Series_Reversion( x/(x + exp(x^3)) ).
 * https://oeis.org/A370876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (3*k+1)^(k-1) *
 *            binomial(n,3*k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq_scalar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq_scalar.h"

/* Small helper: compute n! for n <= 10000 */
static seq_t factorial_uint64(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* preliminary data */
    seq_t result = 0;
    seq_t fac_n = factorial_uint64(n);          /* n!  */
    idx_t max_k = n / 3;                        /* floor(n/3) */

    /* factorial of k, starting with 0! = 1 */
    seq_t fac_k = 1;

    /* binomial coefficient C(n, m), start with m = 0 => 1 */
    seq_t binom = 1;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* update binomial for next k (if needed) */
        if (k > 0)
        {
            idx_t m_prev = 3 * (k - 1);
            /* multiply by (n - m_prev) * (n - m_prev - 1) * (n - m_prev - 2) */
            binom *= (seq_t)(n - m_prev);
            binom *= (seq_t)(n - m_prev - 1);
            binom *= (seq_t)(n - m_prev - 2);

            /* divide by (m_prev + 1) * (m_prev + 2) * (m_prev + 3) = (3k-2)(3k-1)(3k) */
            binom /= (seq_t)(3 * k - 2);
            binom /= (seq_t)(3 * k - 1);
            binom /= (seq_t)(3 * k);
        }

        seq_t term;
        if (k == 0)
        {
            /* k = 0 term: just n! */
            term = fac_n;
        }
        else
        {
            /* compute n! * C(n,3k) */
            term = fac_n * binom;

            /* divide by k! */
            term = term / fac_k;

            /* multiply by (3k+1)^(k-1) */
            term = term * pow_seq_scalar((seq_t)(3 * k + 1), k - 1);
        }

        result += term;

        /* prepare factorial of next k (k+1)! for next loop iteration */
        fac_k *= (seq_t)(k + 1);
    }

    return result;
}
