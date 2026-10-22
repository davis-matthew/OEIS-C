/*
 * A400202  Number of integers k, 1 <= k < n, such that (n+k)/gcd(n,k) is
 *          prime.
 * https://oeis.org/A400202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d>1} (pi(2*d-1) - pi(d))
 * Range:     u8 n <= 479, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;

    /* Enumerate all divisors d of n */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            idx_t d = i;
            if (d > 1)
            {
                seq_t pi_d   = compute_pi(d);
                seq_t pi_2d  = compute_pi((idx_t)(2 * d - 1));
                sum += (pi_2d - pi_d);
            }

            /* paired divisor n/i */
            idx_t d2 = n / i;
            if (d2 != d && d2 > 1)
            {
                seq_t pi_d2  = compute_pi(d2);
                seq_t pi_2d2 = compute_pi((idx_t)(2 * d2 - 1));
                sum += (pi_2d2 - pi_d2);
            }
        }
    }

    return sum;
}
