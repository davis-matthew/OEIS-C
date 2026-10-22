/*
 * A383355  Expansion of 1/sqrt( (1-x) * (1-x-4*x^4) ).
 * https://oeis.org/A383355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) * binomial(n-3*k,k)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* compute term = binom(2*k, k) * binom(n-3*k, k) */
        seq_t term = binomial(2 * k, k) * binomial(n - 3 * k, k);
        sum += term;
    }
    return sum;
}
