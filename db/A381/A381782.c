/*
 * A381782  G.f. A(x) satisfies A(x) = (1 + x*A(x)^3) * C(x), where C(x) is
 *          the g.f. of A000108.
 * https://oeis.org/A381782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n-k+1, k) * binomial(3*n-3*k+1,
 *            n-k)/(3*n-k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
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
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t a1 = 3 * n - k + 1;
        idx_t b1 = k;
        idx_t a2 = 3 * n - 3 * k + 1;
        idx_t b2 = n - k;

        seq_t bin1 = binomial(a1, b1);
        seq_t bin2 = binomial(a2, b2);

        seq_t denom = (seq_t)a1;

        seq_t g1 = gcd(bin1, denom);
        seq_t r1 = bin1 / g1;
        seq_t d1 = denom / g1;

        seq_t g2 = gcd(bin2, d1);
        seq_t r2 = bin2 / g2;
        /* d2 = d1 / g2; left over denominator should be 1 */
        sum += r1 * r2;
    }
    return sum;
}
