/*
 * A344459  a(n) = Sum_{d|n} d^Omega(d).
 * https://oeis.org/A344459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d^Ω(d) (Ω(d) = number of prime factors of d with
 *            multiplicity)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* contribution of divisor d */
            sum += big_pow((seq_t)d, bigomega(d));

            /* contribution of complementary divisor d2 if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
                sum += big_pow((seq_t)d2, bigomega(d2));
        }
    }
    return sum;
}
