/*
 * A365113  G.f. satisfies A(x) = 1 + x / (1 - x*A(x))^3.
 * https://oeis.org/A365113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-k+1,k) *
 *            binomial(n+2k-1,n-k)/(n-k+1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a recurrence for the summand derived from this closed form. */

#include "../utils/gcd.h"

/* Compute a(n) for user supplied types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0)
        return (seq_t)1;

    /* Maximum k such that the first binomial is non‑zero. */
    idx_t kmax = (n + 1) / 2;          /* floor((n+1)/2) */

    /* First non‑zero summand (k = 1). */
    seq_t term = ((seq_t)n * ((seq_t)n + 1)) / 2;   /* n(n+1)/2 */
    seq_t sum  = term;

    for (idx_t k = 1; k < kmax; ++k) {
        /* Numerator of ratio term_{k+1}/term_k . */
        seq_t a1 = (seq_t)(n - 2 * k);
        seq_t a2 = (seq_t)(n - 2 * k + 1);
        seq_t a3 = (seq_t)(n + 2 * k + 1);
        seq_t a4 = (seq_t)(n + 2 * k);

        seq_t numerator   = a1 * a2 * a3 * a4;

        /* Denominator of the ratio. */
        seq_t d1 = (seq_t)(k + 1);
        seq_t d2 = (seq_t)(3 * k + 2);
        seq_t d3 = (seq_t)(3 * k + 1);
        seq_t d4 = (seq_t)(3 * k);

        seq_t denom = d1 * d2 * d3 * d4;

        /* Reduce to avoid overflow: divide out common factors with term and numerator. */
        seq_t g;

        g = gcd(term, denom);
        term  /= g;
        denom /= g;

        g = gcd(numerator, denom);
        numerator /= g;
        denom /= g;

        /* Compute next term. */
        term = (term * numerator) / denom;

        sum += term;
    }

    return sum;
}
