/*
 * A367261  G.f. satisfies A(x) = 1 + x*A(x) * (1 + x*A(x)^2)^3.
 * https://oeis.org/A367261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(2*n - k + 1, k) * binom(3*k, n - k) /
 *            (2*n - k + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binom(2*n - k + 1, k);
        seq_t c2 = binom(3*k, n - k);
        seq_t over = (seq_t)(2*n - k + 1);
        seq_t term = (c1 * c2) / over;
        sum += term;
    }
    return sum;
}
