/*
 * A392554  Expansion of 1 / ((1-x)^2 - x^6)^2.
 * https://oeis.org/A392554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} (k+1) * binomial(n-4*k+3, n-6*k)
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 47, u64 n <= 104, u128 n <= 218,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 6;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t r = n - 4 * k + 3;
        idx_t s = n - 6 * k;
        seq_t term = binomial(r, s);
        sum += term * (seq_t)(k + 1);
    }
    return sum;
}
