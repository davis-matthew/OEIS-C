/*
 * A370285  Coefficient of x^n in the expansion of ( (1+x)^2 + x^3 )^n.
 * https://oeis.org/A370285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(2*n-2*k,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t c1 = binomial(n, k);
        idx_t m = 2 * n - 2 * k;   /* argument of the second binomial   */
        idx_t r = n - 3 * k;
        seq_t c2 = binomial(m, r);
        sum += c1 * c2;
    }
    return sum;
}
