/*
 * A365122  G.f. A(x) satisfies A(x) = (1 + x / (1 - x*A(x))^3)^3.
 * https://oeis.org/A365122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * Sum_{k=0..n} C(3*(n-k+1), k) * C(n+2k-1, n-k) / (n-k+1)
 *            * (from the recurrence with s=3, t=3) * The code implements this
 *            closed form directly
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/seq_gcd.h"    /* seq_t seq_gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* m = n - k + 1, the denominator of the fraction */
        idx_t m = n - k + 1;

        /* first binomial factor: C(3*m, k) */
        seq_t coeff1 = binomial(3 * m, k);

        /* second binomial factor: C(n + 2*k - 1, n - k) */
        /* For k = 0, n - k = n, so the binomial is zero unless n = 0
           (which we handled separately). */
        seq_t coeff2 = binomial(n + 2 * k - 1, n - k);

        /* denominator as a seq_t */
        seq_t denom = (seq_t)m;

        /* reduce coeff1 with denom */
        seq_t g = seq_gcd(coeff1, denom);
        if (g != 0) {
            coeff1 = coeff1 / g;
            denom  = denom / g;
        }

        /* reduce coeff2 with the remaining denom */
        g = seq_gcd(coeff2, denom);
        if (g != 0) {
            coeff2 = coeff2 / g;
            denom  = denom / g;
        }

        /* at this point denom divides (coeff1*coeff2) exactly */
        seq_t prod = coeff1 * coeff2;
        seq_t term = prod / denom;

        sum += term;
    }

    return sum;
}
