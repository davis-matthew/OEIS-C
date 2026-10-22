/*
 * A361719  a(n) = Sum_{k = 1..n} (-1)^(n+k) * k^3 * binomial(n,k)^2.
 * https://oeis.org/A361719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^((n-1)*(n+2)/2) * n * floor((n+1)/2)^2 * binomial(n,
 *            floor(n/2))
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 22, i64 n <= 51, i128 n <= 112,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n = 0 handled naturally */
    idx_t k = (n + 1) >> 1;                     /* floor((n+1)/2) */
    seq_t bin = binomial(n, k);                 /* binomial(n, k) */

    /* compute n * k^2 * binomial(...) */
    seq_t res = (seq_t)n * (seq_t)k * (seq_t)k * bin;

    /* parity of ((n-1)*(n+2)/2) = ((n^2 + n)/2 - 1) */
    /* t = ((n^2 + n) >> 1) */
    idx_t t = ((n * n + n) >> 1);
    /* exponent_mod2 = t ^ 1  (xor 1 flips parity) */
    seq_t sign = ((t & 1) ^ 1) ? -1 : 1;

    return sign * res;
}
