/*
 * A372476  Coefficient of x^n in the expansion of 1 / ( (1-x) * (1-x-x^3)
 *          )^n.
 * https://oeis.org/A372476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(n+k-1,k) * binomial(3*n-2*k-1,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
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

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(3 * n - 2 * k - 1, n - 3 * k);
        sum += t1 * t2;
    }

    return sum;
}
