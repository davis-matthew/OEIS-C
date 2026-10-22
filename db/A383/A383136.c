/*
 * A383136  a(n) = Sum_{k=0..n} k^2 * 2^(n-k) * binomial(n,k).
 * https://oeis.org/A383136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-2) * n * (2 + n) for n >= 2 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    /* n >= 2 */
    seq_t factor = pow3(n - 2);           /* 3^(n-2) */
    seq_t mult = (seq_t)n * ( (seq_t)n + 2 ); /* n * (n+2) */
    return factor * mult;
}
