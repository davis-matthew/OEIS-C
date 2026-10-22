/*
 * A382069  Row sums of the triangular array in A199408.
 * https://oeis.org/A382069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(n) + A000290(n) - A018804(n) Where A018804(n) =
 *            sum_{i=1}^{n} gcd(n,i) = sum_{d|n} d * φ(n/d) and φ is Euler's
 *            totient function
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* should not happen */

    /* triangular number n*(n+1)/2 */
    const seq_t tn = (seq_t)n;
    const seq_t tri = tn * (tn + (seq_t)1) / (seq_t)2;

    /* square */
    const seq_t sq = tn * tn;

    /* sum of gcd(n,i) for i=1..n via divisor summatory formula */
    seq_t sum_gcd = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            const idx_t d1 = d;
            const idx_t d2 = n / d;

            /* d1 * φ(n/d1) */
            sum_gcd += (seq_t)d1 * phi(d2);

            if (d1 != d2)
                sum_gcd += (seq_t)d2 * phi(d1);
        }
    }

    return tri + sq - sum_gcd;
}
