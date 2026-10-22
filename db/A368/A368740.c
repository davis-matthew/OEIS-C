/*
 * A368740  a(n) = Sum_{k = 1..n} gcd(5*k, n).
 * https://oeis.org/A368740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k=1..n} gcd(5*k, n)
 * Range:     u8 n <= 29, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    seq_t N = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = (seq_t)5 * (seq_t)k;
        seq_t g = gcd(val, N);
        sum += g;
    }
    return sum;
}
