/*
 * A382495  a(n) = Sum_{k=0..floor(n/2)} binomial(k+3,3) *
 *          binomial(2*k,2*n-4*k).
 * https://oeis.org/A382495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+3,3) *
 *            binomial(2*k,2*n-4*k)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term1 = binomial(k + 3, 3);
        idx_t m = 2 * n - 4 * k;
        seq_t term2 = binomial(2 * k, m);
        sum += term1 * term2;
    }
    return sum;
}
