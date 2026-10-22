/*
 * A337003  a(n) = Sum_{k=1..n, gcd(n,k) = 1} binomial(n,k)^2.
 * https://oeis.org/A337003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} binomial(n,k)^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = (seq_t)n;          /* binomial(n,1) */
    for (idx_t k = 1; k <= n; ++k)
    {
        if (gcd((seq_t)n, (seq_t)k) == 1)
            sum += bin * bin;

        if (k == n) break;        /* avoid computing bin beyond k=n */

        bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
