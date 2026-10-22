/*
 * A392642  Expansion of 1 / sqrt((1-x)^6 - 4*x^2).
 * https://oeis.org/A392642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*k,k) * binomial(n+4*k+2,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 61,
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
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(2 * k, k);
        seq_t t2 = binomial(n + 4 * k + 2, n - 2 * k);
        sum += t1 * t2;
    }
    return sum;
}
