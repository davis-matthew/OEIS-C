/*
 * A368741  a(n) = Sum_{k = 1..n} gcd(5*k + 1, n).
 * https://oeis.org/A368741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k = 1..n} gcd(5*k + 1, n)
 * Range:     u8 n <= 50, u16 n <= 4031, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t v = ((seq_t)k * 5) + 1;
        sum += gcd(v, n_seq);
    }
    return sum;
}
