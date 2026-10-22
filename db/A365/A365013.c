/*
 * A365013  E.g.f. satisfies A(x) = exp( x*A(x)/(1 - x * A(x)^3) ).
 * https://oeis.org/A365013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} binomial(n-1, k-1) * (3*n-2*k+1)^(k-1)
 *            / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1u;

    /* compute n! */
    seq_t fact_n = (seq_t)1u;
    for (idx_t i = 1u; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t cur = fact_n;      /* fact_n / k! for current k */
    seq_t binom = (seq_t)1u;  /* binomial(n-1, k-1) */
    seq_t sum = (seq_t)0u;

    for (idx_t k = 1u; k <= n; ++k)
    {
        seq_t base = (seq_t)(3u * n - 2u * k + 1u);
        seq_t pow_val = big_pow(base, (seq_t)(k - 1u));

        /* term = binom * pow_val * cur  (fact_n / k!)  */
        seq_t term = binom * pow_val;
        term = term * cur;
        sum += term;

        /* update cur to fact_n / (k+1)!  */
        if (k < n)
            cur /= (seq_t)(k + 1u);

        /* update binom to binomial(n-1, k) */
        if (k < n)
            binom = (binom * (seq_t)(n - k)) / (seq_t)k;
    }

    return sum;  /* already includes the factorial factor */
}
