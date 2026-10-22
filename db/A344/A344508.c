/*
 * A344508  a(n) = Sum_{k=1..n} k * lcm(k,n).
 * https://oeis.org/A344508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * lcm(k,n)
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 346, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t seq_n = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t seq_k = (seq_t)k;
        seq_t g = gcd(seq_k, seq_n);
        seq_t l = (seq_k / g) * seq_n;     /* lcm(k,n) */
        sum += seq_k * l;                  /* k * lcm(k,n) */
    }
    return sum;
}
