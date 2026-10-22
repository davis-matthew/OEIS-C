/*
 * A379249  G.f. A(x) satisfies A(x) = (1 + x*A(x)^3) * (1 + x*A(x))^2.
 * https://oeis.org/A379249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+1, k) * binomial(2*n+4*k+2,
 *            n-k) / (n+2*k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = binomial(n + 2*k + 1, k);
        seq_t bin2 = binomial(2*n + 4*k + 2, n - k);
        seq_t denom = n + 2*k + 1;
        seq_t term = (bin1 * bin2) / denom;
        sum += term;
    }
    return sum;
}
