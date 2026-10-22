/*
 * A308366  Expansion of Sum_{k>=1} (-1)^(k+1)*k*x^k/(1 - k*x^k).
 * https://oeis.org/A308366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(d+1) * d^(n/d)
 * Range:     i8 n <= 14, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n is unsigned, but the formula is defined for n >= 1 */
    seq_t sum = 0;
    /* iterate over divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t exp1 = n / d;
            seq_t term1 = seq_pow((seq_t)d, exp1);
            if ((d & 1) == 0) /* even d => sign negative */
                term1 = -term1;
            sum += term1;

            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t exp2 = n / d2; /* exp2 = d */
                seq_t term2 = seq_pow((seq_t)d2, exp2);
                if ((d2 & 1) == 0)
                    term2 = -term2;
                sum += term2;
            }
        }
    }
    return sum;
}
