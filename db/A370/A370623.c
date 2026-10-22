/*
 * A370623  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^2)
 *          )^(3*n).
 * https://oeis.org/A370623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(3*n+k-1, k) * binomial(n-k-1,
 *            n-2*k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxk = n / (idx_t)2;

    for (idx_t k = (idx_t)0; k <= maxk; ++k) {
        idx_t n1 = (idx_t)3 * n + k - (idx_t)1;
        seq_t p1 = binomial(n1, k);

        idx_t n2 = n - k - (idx_t)1;
        idx_t r2 = n - (idx_t)2 * k;
        seq_t p2 = binomial(n2, r2);

        sum += p1 * p2;
    }

    return sum;
}
