/*
 * A344195  a(n) = Sum_{k=1..n} tau(gcd(k,n))^(n/gcd(k,n)), where tau(n) is
 *          the number of divisors of n.
 * https://oeis.org/A344195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} phi(n/d) * tau(d)^(n/d)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;                 /* d is not a divisor of n */

        idx_t d1 = d;
        idx_t d2 = n / d;             /* complementary divisor */

        /* divisor d1 */
        seq_t phi1 = calc_phi((seq_t)(n / d1));
        seq_t tau1 = (seq_t)divisor_count_u64(d1);
        seq_t pow1 = big_pow(tau1, (seq_t)(n / d1));
        sum += phi1 * pow1;

        /* complementary divisor d2, if distinct */
        if (d1 != d2)
        {
            seq_t phi2 = calc_phi((seq_t)(n / d2));
            seq_t tau2 = (seq_t)divisor_count_u64(d2);
            seq_t pow2 = big_pow(tau2, (seq_t)(n / d2));
            sum += phi2 * pow2;
        }
    }

    return sum;
}
