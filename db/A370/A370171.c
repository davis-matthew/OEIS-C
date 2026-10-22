/*
 * A370171  Coefficient of x^n in the expansion of ( (1+x) * (1+x+x^2)^3 )^n.
 * https://oeis.org/A370171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n,k) * binomial(4*n-k,
 *            n-2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n >> 1;          /* floor(n/2) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t term1 = binomial((idx_t)(3 * n), k);
        seq_t term2 = binomial((idx_t)(4 * n - k), (idx_t)(n - 2 * k));
        sum += term1 * term2;
    }

    return sum;
}
