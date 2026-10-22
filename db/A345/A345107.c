/*
 * A345107  a(n) = Sum_{k=1..n} (-k)^(n - floor(n/k)).
 * https://oeis.org/A345107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-k)^(n - floor(n/k))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t e   = n - n/k;           /* exponent n - floor(n/k) */
        seq_t base = -(seq_t)k;        /* negative base -k */
        seq_t term  = big_pow(base, (seq_t)e);
        sum += term;
    }
    return sum;
}
