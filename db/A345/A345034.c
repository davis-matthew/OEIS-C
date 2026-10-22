/*
 * A345034  a(n) = Sum_{k=1..n} (-2)^(floor(n/k) - 1).
 * https://oeis.org/A345034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-2)^(floor(n/k) - 1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow_minus2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_minus2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for non‑OEIS calls */
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t v = 1; v <= n; ) {
        idx_t m       = n / v;                 /* floor(n/v) */
        idx_t next_v  = n / m + 1;             /* next start index */
        idx_t count   = next_v - v;
        seq_t power   = pow_minus2(m - 1);     /* (-2)^(m-1) */
        sum += (seq_t)count * power;
        v = next_v;
    }
    return sum;
}
