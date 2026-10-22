/*
 * A340626  a(n) = Sum_{d|n, d odd} binomial(d+n/d-1, d).
 * https://oeis.org/A340626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d odd} binomial(d + n/d - 1, d)
 * Range:     u8 n <= 23, u16 n <= 89, u32 n <= 314, u64 n <= 1146,
 *            u128 n <= 4354, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"  /* provided binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d)        /* iterate over divisors up to sqrt(n) */
    {
        if (n % d == 0)                      /* d is a divisor of n */
        {
            idx_t q = n / d;                /* complementary divisor q = n/d */

            if (d & 1)                       /* d odd */
                result += binomial(d + q - 1, d);
            if ((q & 1) && (q != d))          /* q odd and distinct from d */
                result += binomial(q + d - 1, q);
        }
    }
    return result;
}
