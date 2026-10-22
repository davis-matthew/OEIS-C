/*
 * A364410  G.f. A(x) satisfies A(x) = 1 + x^2 * (A(x) / (1 - x))^4.
 * https://oeis.org/A364410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+2*k-1, n-2*k) *
 *            binomial(4*k, k) / (3*k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, and for n > 0 the sum starts from k = 0 */
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t left  = binom(n + 2 * k - 1, n - 2 * k);
        seq_t right = binom(4 * k, k);
        seq_t prod  = left * right;
        seq_t denom = (seq_t)(3 * k + 1);
        prod /= denom;
        sum += prod;
    }

    return sum;
}
