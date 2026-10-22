/*
 * A366939  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+3,4) * floor(n/k).
 * https://oeis.org/A366939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+3,4) * floor(n/k)
 * Range:     i8 n <= 4, i16 n <= 19, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t coeff = binomial(k + 3, 4);
        seq_t q      = (seq_t)(n / k);          /* floor(n/k) */
        seq_t term   = coeff * q;
        if (k & 1)
            sum += term;                      /* odd k: plus */
        else
            sum -= term;                      /* even k: minus */
    }
    return sum;
}
