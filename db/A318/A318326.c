/*
 * A318326  a(n) = Sum_{d|n} [moebius(n/d) < 0]*(sigma(d)-d).
 * https://oeis.org/A318326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [mobius(n/d) == -1]*(sigma(d)-d)
 * Range:     u8 n <= 179, u16 n <= 9682, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   mobius, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 safely */
        return 0;

    seq_t sum = 0;

    /* iterate over divisors d of n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        seq_t mu1 = mobius(n / d1);
        if (mu1 == (seq_t)-1)          /* μ(n/d) == -1 */
            sum += sigma_of(d1) - d1;

        if (d1 != d2) {               /* second divisor, if distinct */
            seq_t mu2 = mobius(n / d2);
            if (mu2 == (seq_t)-1)
                sum += sigma_of(d2) - d2;
        }
    }

    return sum;
}
