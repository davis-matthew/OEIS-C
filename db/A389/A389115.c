/*
 * A389115  Expansion of g^3/(1 + x*g)^2, where g = 1+x*g^2 is the g.f. of
 *          A000108.
 * https://oeis.org/A389115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (k+1) * (k+3) *
 *            binomial(2*n-k+3,n-k)/(2*n-k+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* or a(n) = (1/(n+3)) * Sum_{k=0..n} (-1)^k * (k+1) * (k+3) * binomial(2*n-k+2,n-k) */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) return (seq_t)1;

    /* Pre‑compute the first binomial term B_0 = C(2n+3, n) */
    seq_t B = binom((idx_t)(2 * n + 3), n);

    /* Accumulators for positive and negative contributions */
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term_k = (k+1)*(k+3)*B / (2n-k+3) */
        seq_t numerator   = B * ((seq_t)(k + 1)) * ((seq_t)(k + 3));
        seq_t denominator = (seq_t)(2 * n - k + 3);
        seq_t term        = numerator / denominator;  /* division is exact */

        if (k & 1)
            neg += term;
        else
            pos += term;

        /* Update B to the next binomial term B_{k+1} for k < n
           B_{k+1} = B_k * (n - k) / (2n - k + 3) */
        if (k < n)
        {
            seq_t numer = B * ((seq_t)(n - k));
            seq_t denom = (seq_t)(2 * n - k + 3);
            B = numer / denom;    /* exact division */
        }
    }

    /* Final result: a(n) = pos - neg (non‑negative) */
    return pos - neg;
}
