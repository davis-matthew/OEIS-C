/*
 * A357567  a(n) = 5*A005259(n) - 14*A005258(n).
 * https://oeis.org/A357567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*A005259(n) - 14*A005258(n) A005259(n) = Σ_{k=0..n}
 *            binomial(n,k)^2 * binomial(n+k,k)^2 A005258(n) = Σ_{k=0..n}
 *            binomial(n,k)^2 * binomial(n+k,k)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum1 = 0;
    seq_t sum2 = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(n, k);
        seq_t b2 = binomial(n + k, k);
        seq_t b1sq = b1 * b1;
        seq_t b2sq = b2 * b2;
        sum1 += b1sq * b2sq;   /* contributes to A005259 */
        sum2 += b1sq * b2;     /* contributes to A005258 */
    }
    return (seq_t)5 * sum1 - (seq_t)14 * sum2;
}
