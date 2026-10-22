/*
 * A337180  a(n) = Sum_{d|n} d * gcd(d,n/d).
 * https://oeis.org/A337180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n} d * gcd(d, n/d)
 * Range:     u8 n <= 63, u16 n <= 5183, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)                    /* not defined for 0, return 0 as guard */
        return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t g = gcd((seq_t)d, (seq_t)e);
            sum += (seq_t)d * g;
            if (d != e)               /* add contribution of e when distinct */
                sum += (seq_t)e * g;
        }
    }
    return sum;
}
