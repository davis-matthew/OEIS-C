/*
 * A395042  Greatest prime dividing arithmetic derivative of n; and for primes
 *          p, a(p)=1.
 * https://oeis.org/A395042
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime divisor of the arithmetic derivative of n,
 *            with the convention that a(p)=1 for primes p
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2, but unused by OEIS */

    seq_t n0 = (seq_t)n;   /* keep the original value for calculations */
    seq_t deriv = 0;
    idx_t temp = n;

    /* factorize n by trial division up to sqrt(n) */
    for (idx_t d = 2; d * d <= temp; ++d)
    {
        if (temp % d == 0)
        {
            idx_t cnt = 0;
            while (temp % d == 0)
            {
                temp /= d;
                ++cnt;
            }
            deriv += (seq_t)cnt * (n0 / (seq_t)d);
        }
    }
    if (temp > 1) /* remaining prime factor */
        deriv += (seq_t)1 * (n0 / (seq_t)temp);

    /* For primes, the derivative is 1, leading to answer 1 */
    if (deriv <= 1)
        return 1;

    /* Compute the greatest prime factor of the derivative.
       gpf is available from the utils and works on uint64_t values. */
    uint64_t gpf_val = gpf((uint64_t)deriv);
    return (seq_t)(gpf_val > 1 ? gpf_val : 1);
}
