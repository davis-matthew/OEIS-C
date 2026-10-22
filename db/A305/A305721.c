/*
 * A305721  Crystal ball sequence for the lattice C_7.
 * https://oeis.org/A305721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..7} binomial(14, 2*k) * binomial(n + k, 7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 21, u64 n <= 526,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Coefficients binomial(14, 2*k) for k = 0..7 */
    static const unsigned int coeff[8] = {1, 91, 1001, 3003, 3003, 1001, 91, 1};

    seq_t sum = 0;
    for (idx_t k = 0; k < 8; ++k)
    {
        seq_t c = (seq_t)coeff[k];
        seq_t b = binomial(n + k, 7);
        sum += c * b;
    }
    return sum;
}
