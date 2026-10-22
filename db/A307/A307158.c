/*
 * A307158  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n,3*k)^2.
 * https://oeis.org/A307158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(n,3*k)^2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 18, i64 n <= 35, i128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r   = 3 * k;
        seq_t b   = binomial(n, r);
        seq_t term = b * b;          /* square of binomial */
        if (k & 1)
            sum -= term;             /* (-1)^k = -1 when k is odd */
        else
            sum += term;
    }
    return sum;
}
