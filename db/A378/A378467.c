/*
 * A378467  Expansion of (1/x) * Series_Reversion( x * (1 - x - x^2/(1 - x)^2)
 *          ).
 * https://oeis.org/A378467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(n+k,k) *
 *            binomial(2*n+k,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t bin1 = binomial(n + k, k);
        seq_t bin2 = binomial(2 * n + k, n - 2 * k);
        sum += bin1 * bin2;
    }
    return sum / (seq_t)(n + 1);
}
