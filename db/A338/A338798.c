/*
 * A338798  a(n) = Sum_{k=1..n-1} lcm(lcm(n, k), lcm(n, n-k)).
 * https://oeis.org/A338798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Sum_{k=1..n-1} k*(n-k)/gcd(n,k)^2
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 400, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    const seq_t seq_n = (seq_t)n;

    for (idx_t k = 1; k < n; ++k)
    {
        const seq_t seq_k   = (seq_t)k;
        const seq_t seq_nk  = seq_n - seq_k;
        const seq_t g       = gcd(seq_n, seq_k);
        const seq_t g2      = g * g;
        const seq_t term    = (seq_k * seq_nk) / g2;
        sum += term;
    }

    return seq_n * sum;
}
