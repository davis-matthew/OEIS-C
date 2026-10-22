/*
 * A382999  a(n) = Sum_{d|n} phi(n/d) * (-2)^(d-1).
 * https://oeis.org/A382999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * (-2)^(d-1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for(idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;

            // term for divisor d
            seq_t term = phi(n / d);
            seq_t pow = pow2(d - 1);
            if ((d - 1) & 1) term = -term;
            sum += term * pow;

            if (d2 != d) {
                // term for paired divisor d2
                seq_t term2 = phi(n / d2);
                seq_t pow2val = pow2(d2 - 1);
                if ((d2 - 1) & 1) term2 = -term2;
                sum += term2 * pow2val;
            }
        }
    }
    return sum;
}
