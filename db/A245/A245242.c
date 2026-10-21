/*
 * A245242  a(n) = Sum_{k=0..n} binomial(n^2 - k^2, n*k - k^2).
 * https://oeis.org/A245242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n^2 - k^2, n*k - k^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = n * n - k * k;       /* n^2 - k^2 = (n-k)(n+k) */
        idx_t K = n * k - k * k;       /* n*k - k^2 = k(n-k)    */
        total += binom(N, K);
    }
    return total;
}
