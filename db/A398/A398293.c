/*
 * A398293  Diagonal of rational function 1/(1 - (1 + x*y*z*w)*(x + y) - z -
 *          w).
 * https://oeis.org/A398293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (4*k)!/k!^4 * binomial(2*k, n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t mult = binomial(4 * k, k);
        mult *= binomial(3 * k, k);
        mult *= binomial(2 * k, k);
        seq_t term = mult * binomial(2 * k, n - k);
        sum += term;
    }
    return sum;
}
