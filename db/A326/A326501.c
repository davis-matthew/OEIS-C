/*
 * A326501  a(n) = Sum_{k=0..n} (-k)^k.
 * https://oeis.org/A326501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \sum_{k=0}^{n} (-k)^k
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base = (seq_t)k;
        seq_t term = pow_int(base, k);     /* base^exp, 0^0 returns 1 */
        if (k & 1)                          /* multiply by (-1)^k */
            term = -term;
        sum += term;
    }
    return sum;
}
