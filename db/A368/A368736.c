/*
 * A368736  a(n) = Sum_{k = 1..n} gcd(2*k+1, n).
 * https://oeis.org/A368736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k = 1..n} gcd(2*k+1, n)
 * Range:     u8 n <= 62, u16 n <= 4724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sn   = (seq_t)n;           /* cast once for efficiency */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t m = (seq_t)k * 2 + 1;  /* 2*k+1 */
        sum += gcd(m, sn);
    }
    return sum;
}
