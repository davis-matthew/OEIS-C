/*
 * A368739  a(n) = Sum_{k = 1..n} gcd(4*k, n).
 * https://oeis.org/A368739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k = 1..n} gcd(4*k, n)
 * Range:     u8 n <= 31, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t N   = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute 4*k; for seq_t sizes that fit in 64 bits this is safe,
           because n is expected to be small enough for the tests. */
        seq_t k4 = 4 * (seq_t)k;
        sum += gcd(k4, N);
    }
    return sum;
}
