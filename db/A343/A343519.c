/*
 * A343519  a(n) = Sum_{1 <= x_1 <= x_2 <= x_3 <= x_4 <= x_5 <= n} gcd(x_1,
 *          x_2, x_3 , x_4, x_5, n).
 * https://oeis.org/A343519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * binomial(d+4, 5)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* term for divisor d1 */
            seq_t term1 = binomial(d1 + 4, 5) * phi(d2);
            sum += term1;

            if (d1 != d2) {
                /* term for divisor d2 */
                seq_t term2 = binomial(d2 + 4, 5) * phi(d1);
                sum += term2;
            }
        }
    }
    return sum;
}
