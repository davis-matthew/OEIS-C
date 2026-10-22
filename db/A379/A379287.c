/*
 * A379287  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^2) * (1 - x*A(x)^6)).
 * https://oeis.org/A379287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+5*k+1,k) *
 *            binomial(3*n+3*k,n-k)/(2*n+5*k+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t a1 = 2 * n + 5 * k + 1;
        idx_t a2 = 3 * n + 3 * k;

        seq_t term1 = binomial(a1, k);
        seq_t term2 = binomial(a2, n - k);

        seq_t denom = (seq_t)a1;
        /* Reduce the denominator against both binomials to avoid overflow */
        seq_t g = gcd(term1, denom);
        term1 /= g;
        denom /= g;

        if (denom != 1) {
            g = gcd(term2, denom);
            term2 /= g;
            denom /= g;
        }

        /* At this point denom should be 1 (division is exact) */
        seq_t prod = term1 * term2;
        if (denom != 1)
            prod /= denom;

        sum += prod;
    }
    return sum;
}
