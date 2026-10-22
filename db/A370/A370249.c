/*
 * A370249  Coefficient of x^n in the expansion of ( 1/(1-x)^2 * (1+x^3)^2
 *          )^n.
 * https://oeis.org/A370249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,k) *
 *            binomial(3*n-3*k-1,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binom((idx_t)(2 * n), k);
        seq_t t2 = binom((idx_t)(3 * n - 3 * k - 1), (idx_t)(n - 3 * k));
        sum += t1 * t2;
    }
    return sum;
}
