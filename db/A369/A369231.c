/*
 * A369231  Expansion of (1/x) * Series_Reversion( x * (1-x)^3 / (1-x+x^3)^2
 *          ).
 * https://oeis.org/A369231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binom(2*n+2, k) *
 *            binom(2*n-2*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = binom(2 * n + 2, k);
        seq_t t2 = binom(2 * n - 2 * k, n - 3 * k);
        sum += t1 * t2;
    }
    return sum / (seq_t)(n + 1);
}
