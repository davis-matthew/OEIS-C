/*
 * A379286  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^2) * (1 - x*A(x)^5)).
 * https://oeis.org/A379286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+4*k+1,k) *
 *            binomial(3*n+2*k,n-k) / (2*n+4*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided utilities binom() and gcd(). */

#include "../utils/binom.h"
#include "../utils/gcd.h"

/* Computes the n-th term of OEIS A379286. */
extern seq_t a(idx_t n)
{
    /* Handle the zero index directly (the loop below would also work). */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* Arguments for the first binomial */
        idx_t a1 = 2 * n + 4 * k + 1;
        idx_t b1 = k;
        seq_t bin1 = binom(a1, b1);

        /* Arguments for the second binomial */
        idx_t a2 = 3 * n + 2 * k;
        idx_t b2 = n - k;
        seq_t bin2 = binom(a2, b2);

        /* Denominator (common divisor) */
        seq_t d = (seq_t)a1;  /* 2*n + 4*k + 1 */

        /* Cancel the divisor with the two binomials to keep intermediate
           values small and avoid overflow for 64‑bit builds. */
        seq_t g = gcd(bin1, d);
        bin1 /= g;
        d /= g;

        g = gcd(bin2, d);
        bin2 /= g;
        d /= g;

        /* After the cancellations, d should be 1, but keep the check for safety. */
        seq_t term = bin1 * bin2;
        if (d != (seq_t)1)
            term /= d;

        sum += term;
    }
    return sum;
}
