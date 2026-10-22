/*
 * A378427  Expansion of (1/x) * Series_Reversion( x / (1 + x + x^3 * (1 +
 *          x)^3) ).
 * https://oeis.org/A378427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * sum_{k=0..floor(n/3)}
 *            binomial(n+1,k)*binomial(n+2*k+1,n-3*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(n + 2 * k + 1, n - 3 * k);
        sum += c1 * c2;
    }
    seq_t denom = (seq_t)n + 1;
    return sum / denom;
}
