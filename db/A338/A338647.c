/*
 * A338647  a(n) = Sum_{k=1..n} 2^(k/gcd(n,k) - 1).
 * https://oeis.org/A338647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^(k/gcd(n,k) - 1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Euclidean algorithm for gcd(k, n) */
        idx_t a = k, b = n;
        while (b != 0)
        {
            idx_t r = a % b;
            a = b;
            b = r;
        }
        idx_t exp = k / a - 1;
        sum += pow2(exp);
    }
    return sum;
}
