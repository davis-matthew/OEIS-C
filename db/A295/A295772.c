/*
 * A295772  a(n) = Sum_{k=0..n} binomial((n-k)*k, k).
 * https://oeis.org/A295772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial((n-k)*k, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = (n - k) * k;
        seq_t term = binomial(m, k);
        total += term;
    }
    return total;
}
