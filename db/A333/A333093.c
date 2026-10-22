/*
 * A333093  a(n) is equal to the n-th order Taylor polynomial (centered at 0)
 *          of c(x)^n evaluated at x = 1, where c(x) = (1 - sqrt(1 -
 *          4*x))/(2*x) is the o.g.f. of the Catalan numbers A000108.
 * https://oeis.org/A333093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n/(n+2*k) * binomial(n+2*k, k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t sn   = (seq_t)n;           /* convert once for efficiency */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* denominator d = n + 2*k */
        seq_t d = sn + (seq_t)(2 * k);

        /* binomial coefficient C(n+2*k, k) */
        seq_t bin = binomial((idx_t)(n + 2 * k), k);

        /* Reduce bin with the denominator */
        seq_t g = gcd(bin, d);
        bin /= g;
        d   /= g;

        /* Now compute term = bin * n / d */
        seq_t tmp = bin * sn;

        /* In case a further common factor remains */
        g = gcd(tmp, d);
        tmp /= g;
        d   /= g;

        /* d should be 1 now; the division is exact */
        sum += tmp / d;
    }

    return sum;
}
