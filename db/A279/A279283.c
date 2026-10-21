/*
 * A279283  Self-composition of the tetrahedral (or triangular pyramidal)
 *          numbers; g.f.: A(x) = G(G(x)), where G(x) = g.f. of A000292.
 * https://oeis.org/A279283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,3) * binomial(n+3*k-1,4*k-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t t = binomial(k + 2, 3);
        seq_t u = binomial(n + 3 * k - 1, 4 * k - 1);
        sum += t * u;
    }
    return sum;
}
