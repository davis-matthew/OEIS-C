/*
 * A370195  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x^2)^2 )^n.
 * https://oeis.org/A370195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n,k) * binomial(2*n,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(2 * n, k);
        seq_t c2 = binomial(2 * n, n - 2 * k);
        result += c1 * c2;
    }
    return result;
}
