/*
 * A359883  a(n) = Sum_{d|n} d^(n-d) * (n/d)^d.
 * https://oeis.org/A359883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d) * (n/d)^d
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* offset starts at 1; guard against 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t result = 0;

    /* iterate over all divisors d <= sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) {
            continue;
        }

        idx_t d2 = n / d;                 /* complementary divisor */

        /* contribution for divisor d */
        seq_t part1 = ipow((seq_t)d, (seq_t)(n - d));
        seq_t part2 = ipow((seq_t)d2, (seq_t)d);
        result += part1 * part2;

        /* contribution for divisor d2 if distinct from d */
        if (d2 != d) {
            seq_t part1b = ipow((seq_t)d2, (seq_t)(n - d2));
            seq_t part2b = ipow((seq_t)d, (seq_t)d2);
            result += part1b * part2b;
        }
    }

    return result;
}
