/*
 * A370172  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x+x^2)^3
 *          )^n.
 * https://oeis.org/A370172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n,k) *
 *            binomial(5*n-k,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial((idx_t)(3 * n), k);
        seq_t t2 = binomial((idx_t)(5 * n - k), (idx_t)(n - 2 * k));
        sum += t1 * t2;
    }
    return sum;
}
