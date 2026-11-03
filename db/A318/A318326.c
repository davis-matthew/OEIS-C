/*
 * A318326  a(n) = Sum_{d|n} [moebius(n/d) < 0]*(sigma(d)-d).
 * https://oeis.org/A318326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [mobius(n/d) == -1]*(sigma(d)-d)
 * Range:     u8 n <= 179, u16 n <= 20159, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9682 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mobius.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
