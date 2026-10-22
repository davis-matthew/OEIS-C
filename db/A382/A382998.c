/*
 * A382998  a(n) = Sum_{d|n} phi(n/d) * (-n)^(d-1).
 * https://oeis.org/A382998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} φ(n/d) * (-n)^(d-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                     /* uint64_t, etc. */
#include <stdbool.h>
#include "../utils/phi.h"                /* seq_t phi(idx_t m); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow_curr = 1;                       /* (-n)^0 */

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {                     /* d is a divisor */
            seq_t ph = phi(n / d);           /* φ(n/d) */
            sum += ph * pow_curr;            /* contribution */
        }
        /* advance to (-n)^d for next d */
        pow_curr *= -(seq_t)n;
    }
    return sum;
}
