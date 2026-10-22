/*
 * A365697  G.f. satisfies A(x) = 1 + x^4*A(x)^3 / (1 - x*A(x)).
 * https://oeis.org/A365697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binom(n-3k-1,
 *            n-4k)*binom(n-k+1,k)/(n-k+1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the first few values directly. */
    if (n == 0) return (seq_t)1;      /* a(0) = 1 */
    if (n < 4)  return (seq_t)0;      /* a(1), a(2), a(3) = 0 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 4; ++k) {
        seq_t b1 = binom(n - 3*k - 1, n - 4*k);
        seq_t b2 = binom(n - k + 1, k);
        seq_t denom = (seq_t)(n - k + 1);
        sum += (b1 * b2) / denom;
    }
    return sum;
}
