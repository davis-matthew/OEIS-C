/*
 * A342167  a(n) = U(n, (n+2)/2) where U(n, x) is a Chebyshev polynomial of
 *          the 2nd kind.
 * https://oeis.org/A342167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} n^k * binomial(n+1+k, 2k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = 1;                     /* n^0 */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n + 1 + k, 2*k + 1);
        sum += power * bin;
        power *= (seq_t)n;
    }
    return sum;
}
