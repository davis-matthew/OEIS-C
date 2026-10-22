/*
 * A380778  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x / (1 +
 *          x)^2) / (1 + x)^2 ).
 * https://oeis.org/A380778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(2*n-2*k+2, n-k)
 *            / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   factorial_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial_impl.h"

/* Compute binomial coefficient C(n,k) as a seq_t using multiplicative formula.
   Assumes 0 <= k <= n. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact division */
    }
    return res;
}

/* Sequence function for A380778. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t fact_n = factorial_impl(n);
    seq_t sum    = 0;

    /* k = 0 term: (n! * C(2n+2, n)) / (n+1) */
    {
        seq_t bin0 = binom_calc(2*n + 2, n);
        sum = sum + (fact_n * bin0) / (seq_t)(n + 1);
    }

    /* k >= 1 terms: (n! / k!) * (n+1)^(k-1) * C(2n-2k+2, n-k) */
    seq_t ratio = fact_n;   /* n! / 0! */
    seq_t power = 1;        /* (n+1)^(k-1), starts at 1 for k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        ratio = ratio / (seq_t)k;                /* n! / k! */
        seq_t bin = binom_calc(2*n - 2*k + 2, n - k);
        sum = sum + (ratio * power) * bin;       /* exact multiplication */
        power = power * (seq_t)(n + 1);          /* next power */
    }

    return sum;
}
