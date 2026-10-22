/*
 * A383000  a(n) = Sum_{d|n} phi(n/d) * (-3)^(d-1).
 * https://oeis.org/A383000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * (-3)^(d-1) Implementation uses the
 *            utilities calc_phi and pow_seq
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   calc_phi, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    seq_t result = 0;
    /* iterate divisors of n */
    for (uint64_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t phi1 = calc_phi((seq_t)(n / d));
            seq_t pow1 = pow_seq((seq_t)-3, (int)(d - 1));
            __int128 term1 = (__int128)phi1 * (__int128)pow1;
            result += (seq_t)term1;

            uint64_t d2 = n / d;
            if (d2 != d)
            {
                /* complementary divisor d2 */
                seq_t phi2 = calc_phi((seq_t)(n / d2));
                seq_t pow2 = pow_seq((seq_t)-3, (int)(d2 - 1));
                __int128 term2 = (__int128)phi2 * (__int128)pow2;
                result += (seq_t)term2;
            }
        }
    }
    return result;
}
