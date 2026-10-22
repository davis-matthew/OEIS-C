/*
 * A356130  a(n) = Sum_{k=1..n} sigma_{n-1}(k).
 * https://oeis.org/A356130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(n-1) * floor(n/k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of OEIS sequence */

    seq_t sum = 0;
    idx_t exp = n - 1;              /* exponent n-1 */

    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t power = seq_pow((seq_t)d, exp);  /* d^(n-1) */
        idx_t mult  = n / d;                   /* floor(n/d) */
        sum += power * (seq_t)mult;
    }
    return sum;
}
