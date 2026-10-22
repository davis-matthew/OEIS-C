/*
 * A319086  a(n) = Sum_{k=1..n} k^2*sigma(k), where sigma is A000203.
 * https://oeis.org/A319086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k² * σ(k) where σ(k) is the sum of divisors of
 *            k
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 319, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined only for n ≥ 1, but guard anyway */

    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sigma = 0;

        /* compute σ(k) by iterating over divisor pairs */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                sigma += (seq_t)d;         /* divisor d */
                idx_t e = k / d;           /* complementary divisor */
                if (e != d)                /* avoid double counting a square root */
                    sigma += (seq_t)e;
            }
        }

        /* term = k² * σ(k) */
        seq_t term = (seq_t)k * (seq_t)k;
        term *= sigma;
        total += term;
    }

    return total;
}
