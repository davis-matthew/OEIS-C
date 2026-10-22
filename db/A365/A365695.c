/*
 * A365695  G.f. satisfies A(x) = 1 + x^3*A(x)^5 / (1 - x*A(x)).
 * https://oeis.org/A365695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2k-1, n-3k) * *
 *            binomial(n+2k+1, k) / (n+2k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 94,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 (the empty composition) */
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t max_k = n / 3;
    /* loop over all admissible k */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* For n < 2k+1 the first binomial has a negative top argument
           – binomial() returns 0 in that case – skip the term. */
        if (n < 2 * k + 1) continue;

        idx_t a1 = n - 2 * k - 1;   /* top of first binomial  */
        idx_t b1 = n - 3 * k;       /* bottom of first binomial */

        idx_t a2 = n + 2 * k + 1;   /* top of second binomial */
        idx_t b2 = k;               /* bottom of second binomial */

        seq_t c1 = binomial(a1, b1);
        if (c1 == 0) continue;      /* whole term vanishes */

        seq_t c2 = binomial(a2, b2);
        /* denominator is a small positive integer */
        seq_t denom = (seq_t)(n + 2 * k + 1);

        /* Cancel all common factors between the denominator
           and the two binomial terms to avoid overflow. */
        seq_t g;

        g = gcd(c1, denom);
        c1 /= g;          /* reduces c1 */
        denom /= g;

        g = gcd(c2, denom);
        c2 /= g;          /* reduces c2 */
        denom /= g;       /* denom should now be 1 */

        /* After the cancellations the product equals the term
           value exactly.  As the final result fits into seq_t,
           this multiplication will not overflow. */
        seq_t term = c1 * c2;

        sum += term;
    }
    return sum;
}
