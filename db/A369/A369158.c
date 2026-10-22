/*
 * A369158  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2+x^4) ).
 * https://oeis.org/A369158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/4⌋} binomial(n+1,k) *
 *            binomial(2*n-2*k+2,n-4*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t lim = n / 4;
    seq_t sum = 0;
    for (idx_t k = 0; k <= lim; ++k) {
        seq_t term1 = binomial(n + 1, k);
        seq_t term2 = binomial(2 * n - 2 * k + 2, n - 4 * k);
        sum += term1 * term2;
    }
    return sum / ((seq_t)n + 1);
}
