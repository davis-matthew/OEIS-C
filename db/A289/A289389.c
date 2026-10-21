/*
 * A289389  a(n) = Sum_{k>=0} (-1)^k*binomial(n,5*k+4).
 * https://oeis.org/A289389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * binomial(n, 5*k+4)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 30, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
