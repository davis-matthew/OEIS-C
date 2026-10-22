/*
 * A345109  a(n) = Sum_{k=1..n} (-2)^(n - floor(n/k)).
 * https://oeis.org/A345109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-2)^(n - floor(n/k))
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 28, i64 n <= 59, i128 n <= 122,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, but guard against 0 */
    seq_t sum = 0;
    seq_t base = (seq_t)-2;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q        = n / k;          /* floor(n/k) */
        idx_t exp_idx  = n - q;          /* exponent n - floor(n/k) */
        sum += big_pow(base, (seq_t)exp_idx);
    }
    return sum;
}
