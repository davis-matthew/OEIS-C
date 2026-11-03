/*
 * A289389  a(n) = Sum_{k>=0} (-1)^k*binomial(n,5*k+4).
 * https://oeis.org/A289389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * binomial(n, 5*k+4)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 35, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4)          /* no term satisfies 5*k+4 <= n */
        return 0;

    seq_t sum = 0;
    int   sign = 1;     /* (-1)^k */

    for (idx_t r = 4; r <= n; r += 5)
    {
        seq_t c = binomial(n, r);
        sum += (sign > 0) ? c : -c;
        sign = -sign;
    }

    return sum;
}
