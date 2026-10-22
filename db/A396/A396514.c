/*
 * A396514  a(n) = Sum_{k=1..n, k does not divide n} gcd(k,n).
 * https://oeis.org/A396514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d*(phi(n/d)-1) which is equivalent to
 *            a(n)=Sum_{k=1..n, k∤n} gcd(k,n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* defined for n≥1, guard for safety */

    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 */
            seq_t phi_d2 = calc_phi((seq_t)d2);
            sum += (seq_t)d1 * (phi_d2 - (seq_t)1);

            /* divisor d2 (if distinct) */
            if (d1 != d2)
            {
                seq_t phi_d1 = calc_phi((seq_t)d1);
                sum += (seq_t)d2 * (phi_d1 - (seq_t)1);
            }
        }
    }

    return sum;
}
