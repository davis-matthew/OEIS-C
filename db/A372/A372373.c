/*
 * A372373  Coefficient of x^n in the expansion of ( (1+x+x^3)^3 / (1+x)^2
 *          )^n.
 * https://oeis.org/A372373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n,k) *
 *            binomial(n-k,n-3*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t t = (idx_t)3 * n;
        seq_t term1 = binomial(t, k);
        idx_t a1 = n - k;
        idx_t b1 = n - 3 * k;
        seq_t term2 = binomial(a1, b1);
        sum += term1 * term2;
    }
    return sum;
}
