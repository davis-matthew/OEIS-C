/*
 * A361699  Constant term in the expansion of (1 + x^3 + y^3 + 1/(x*y))^n.
 * https://oeis.org/A361699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} binomial(2*k, k) * binomial(5*k, 2*k) *
 *            binomial(n, 5*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 5;
    seq_t result = 0;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t term = binomial(2 * k, k);
        term *= binomial(5 * k, 2 * k);
        term *= binomial(n, 5 * k);
        result += term;
    }

    return result;
}
