/*
 * A376490  G.f. satisfies A(x) = 1 / (1 - x^3*A(x)^3 / (1 - x)).
 * https://oeis.org/A376490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(4*k,k) *
 *            binomial(n-2*k-1,n-3*k) / (3*k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* small offsets */
    if (n == 0)
        return (seq_t)1;
    if (n < 3)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t kmax = n / 3;

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t fourk = 4 * k;
        seq_t c1 = binom(fourk, k);

        idx_t n2k1 = n - 2 * k - 1;
        idx_t n3k  = n - 3 * k;
        seq_t c2 = binom(n2k1, n3k);

        seq_t term = c1 * c2;
        seq_t denom = (seq_t)(3 * k + 1);
        term /= denom;
        sum += term;
    }
    return sum;
}
