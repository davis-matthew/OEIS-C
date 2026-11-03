/*
 * A381881  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2 * C(x)) ),
 *          where C(x) is the g.f. of A000108.
 * https://oeis.org/A381881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n+2k+1,k) * binomial(2n+2,n-k) /
 *            (n+2k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
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
        idx_t denomIdx = n + 2 * k + 1;
        seq_t c1 = binomial(denomIdx, k);
        seq_t c2 = binomial(2 * n + 2, n - k);
        seq_t denom = (seq_t)denomIdx;

        seq_t g = gcd(c1, denom);
        c1 /= g;
        denom /= g;

        g = gcd(c2, denom);
        c2 /= g;
        denom /= g;

        seq_t term = c1 * c2;
        if (denom != (seq_t)1) {
            term /= denom;   /* exact division */
        }
        sum += term;
    }
    return sum;
}
