/*
 * A365818  G.f. A(x) satisfies: A(x) = x * (1 + A(x))^3 / (1 - 5 * A(x)).
 * https://oeis.org/A365818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} binom(n+k-1,k) * binom(3*n, n-k-1)
 *            * 5^k (A365818)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t, seq_t are supplied by the build */

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    seq_t sum  = (seq_t)0;
    seq_t B    = (seq_t)1;                                /* binom(n-1,0)   */
    seq_t C    = binom_nk((idx_t)(3*n), (idx_t)(n-1));     /* binom(3n,n-1)  */
    seq_t pow5 = (seq_t)1;

    for (idx_t k = (idx_t)0; k < n; ++k)
    {
        seq_t term = B * C * pow5;
        sum += term;

        /* update for next k if any */
        if (k + (idx_t)1 < n)
        {
            B = B * (seq_t)(n + k) / (seq_t)(k + (idx_t)1);

            seq_t num = (seq_t)(n - k - (idx_t)1);
            seq_t den = (seq_t)(2*n + k + (idx_t)2);
            C = C * num / den;

            pow5 = pow5 * (seq_t)5;
        }
    }

    return sum / (seq_t)n;   /* division is exact */
}
