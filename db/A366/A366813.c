/*
 * A366813  a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+2,3).
 * https://oeis.org/A366813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+2,3)
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 2918, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9890 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1 */
        return 0;

    seq_t pos = 0;            /* sum of terms with positive sign */
    seq_t neg = 0;            /* sum of terms with negative sign */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d1;

        /* first divisor d1 */
        seq_t term1 = binomial(d1 + 2, 3);
        idx_t exp1 = d2 - 1;          /* n/d1 - 1 == d2 - 1 */
        if ((exp1 & 1) == 0)
            pos += term1;
        else
            neg += term1;

        /* paired divisor d2, if distinct */
        if (d2 != d1) {
            seq_t term2 = binomial(d2 + 2, 3);
            idx_t exp2 = d1 - 1;      /* n/d2 - 1 == d1 - 1 */
            if ((exp2 & 1) == 0)
                pos += term2;
            else
                neg += term2;
        }
    }

    return pos - neg;          /* pos >= neg for this sequence */
}
