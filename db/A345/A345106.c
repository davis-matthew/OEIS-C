/*
 * A345106  a(n) = Sum_{k=1..n} k^(n - floor(n/k)).
 * https://oeis.org/A345106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(n - floor(n/k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q   = n / k;          /* floor(n/k) */
        idx_t exp = n - q;          /* exponent: n - floor(n/k) */
        seq_t term = big_pow((seq_t)k, (seq_t)exp);
        sum += term;
    }
    return sum;
}
