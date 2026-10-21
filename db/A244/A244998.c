/*
 * A244998  Number of ways for five teams of a World Cup football group to
 *          each have n goals for and n goals against.
 * https://oeis.org/A244998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{8} coeffs[k] * binomial(n - k + 11, 11), coeffs
 *            = {1,32,408,1724,2765,1724,408,32,1}
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static const unsigned int coeffs[9] = {1, 32, 408, 1724, 2765, 1724, 408, 32, 1};
    seq_t result = 0;
    for (idx_t k = 0; k < 9 && n >= k; ++k)
    {
        idx_t m = n - k + 11;            // m ≥ 11 because n ≥ k
        seq_t comb = binomial(m, 11);
        seq_t term = mul_small(comb, coeffs[k]);
        result += term;
    }
    return result;
}
