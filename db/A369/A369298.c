/*
 * A369298  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 * (1-x^3)^2 ).
 * https://oeis.org/A369298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(2*n+k+1, k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b1 = binomial(2 * n + k + 1, k);
        seq_t b2 = binomial(3 * n - 3 * k + 1, n - 3 * k);
        seq_t prod = seq_mul(b1, b2);
        sum = sum + prod;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
