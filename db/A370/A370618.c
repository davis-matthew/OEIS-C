/*
 * A370618  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^2)^2 )^n.
 * https://oeis.org/A370618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k-1,k) *
 *            binomial(2*n-k-1,n-2*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t total = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n1 = 2 * n + k - 1;
        idx_t n2 = 2 * n - k - 1;
        idx_t k1 = k;
        idx_t k2 = n - 2 * k;
        seq_t b1 = binomial(n1, k1);
        seq_t b2 = binomial(n2, k2);
        total += b1 * b2;
    }
    return total;
}
