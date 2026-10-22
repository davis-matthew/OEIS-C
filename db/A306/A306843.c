/*
 * A306843  a(n) = Sum_{d|n} binomial(n,d)^3.
 * https://oeis.org/A306843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n,d)^3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* below the offset: return 0 */
    seq_t total = 0;

    /* iterate over divisors d of n up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;      /* not a divisor */

        /* first divisor d */
        seq_t t = binomial(n, d);
        total += t * t * t;

        /* paired divisor n/d, avoid double‑counting square root */
        idx_t other = n / d;
        if (other != d) {
            t = binomial(n, other);
            total += t * t * t;
        }
    }

    return total;
}
