/*
 * A338682  a(n) = Sum_{d|n} (-1)^(d-1) * binomial(d+n/d-1, d).
 * https://oeis.org/A338682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * binomial(d + n/d - 1, d)
 * Range:     i8 n <= 17, i16 n <= 79, i32 n <= 287, i64 n <= 1119,
 *            i128 n <= 4306, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0)
            continue;

        /* divisor d */
        seq_t term = binomial(d + n / d - 1, d);
        if ((d & 1) == 0)          /* even d => sign = -1 */
            term = -term;
        total += term;

        idx_t d2 = n / d;
        if (d2 != d)
        {
            seq_t term2 = binomial(d2 + n / d2 - 1, d2);
            if ((d2 & 1) == 0)
                term2 = -term2;
            total += term2;
        }
    }
    return total;
}
