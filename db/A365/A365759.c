/*
 * A365759  G.f. satisfies A(x) = 1 + x*A(x)*(1 + x^3*A(x)^5).
 * https://oeis.org/A365759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(n-3k, k) * binomial(n+2k+1, n-3k)
 *            / (n+2k+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n - 3 * k, k);
        seq_t term2 = binomial(n + 2 * k + 1, n - 3 * k);
        seq_t denom = (seq_t)(n + 2 * k + 1);
        seq_t prod  = term1 * term2;
        prod /= denom;
        sum += prod;
    }
    return sum;
}
