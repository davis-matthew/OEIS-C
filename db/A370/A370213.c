/*
 * A370213  Coefficient of x^n in the expansion of ( (1+x)^2 / (1-x^3) )^n.
 * https://oeis.org/A370213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+k-1,k) * binomial(2*n,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
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
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(2 * n, n - 3 * k);
        sum += t1 * t2;
    }
    return sum;
}
