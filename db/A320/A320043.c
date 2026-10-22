/*
 * A320043  Row sums of the triangle A322550.
 * https://oeis.org/A320043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n+1-k)^2*k/gcd(n+1-k, k)^3
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 477, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t nk = (seq_t)(n + 1 - k);   /* n+1-k as seq_t */
        seq_t kk = (seq_t)k;
        seq_t g  = gcd(nk, kk);
        seq_t denom = g * g * g;         /* g^3 */
        seq_t term  = nk * nk * kk / denom;
        sum += term;
    }
    return sum;
}
