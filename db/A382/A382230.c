/*
 * A382230  a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A382230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(2*k,2*n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 85,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t two_n = 2 * n;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(k + 2, 2);
        term *= binomial(2 * k, two_n - 2 * k);
        sum += term;
    }
    return sum;
}
