/*
 * A370159  Coefficient of x^n in the expansion of ( (1+x) * (1+x+x^2)^2 )^n.
 * https://oeis.org/A370159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n,k) * binomial(3*n-k,
 *            n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    idx_t kmax  = n / 2;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t t1 = binomial(2 * n, k);
        seq_t t2 = binomial(3 * n - k, n - 2 * k);
        sum += t1 * t2;
    }
    return sum;
}
