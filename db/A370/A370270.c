/*
 * A370270  Coefficient of x^n in the expansion of 1/( (1-x)^2 * (1-x^2)^3
 *          )^n.
 * https://oeis.org/A370270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n+k-1,k) *
 *            binomial(3*n-2*k-1,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(3*n + k - 1, k);
        seq_t term2 = binomial(3*n - 2*k - 1, n - 2*k);
        sum += term1 * term2;
    }
    return sum;
}
