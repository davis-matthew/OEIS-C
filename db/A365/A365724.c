/*
 * A365724  G.f. A(x) satisfies A(x) = 1 + x^3*A(x)^3*(1 + x*A(x)).
 * https://oeis.org/A365724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} C(k, n-3k) * C(n+1, k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t c1 = binom(k, n - 3 * k);
        seq_t c2 = binom(n + 1, k);
        sum += c1 * c2;
    }
    return sum / (seq_t)(n + 1);
}
