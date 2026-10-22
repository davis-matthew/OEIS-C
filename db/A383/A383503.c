/*
 * A383503  Expansion of 1/sqrt( (1-x) * (1-x-4*x^3)^3 ).
 * https://oeis.org/A383503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (2*k+1) * binomial(2*k,k) *
 *            binomial(n-2*k+1,k+1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Computes A383503: expansion of 1/sqrt((1-x)*(1-x-4*x^3)^3) */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t c1 = binom(2 * k, k);                       /* binomial(2k, k)   */
        seq_t c2 = binom(n - 2 * k + 1, k + 1);           /* binomial(n-2k+1, k+1) */
        seq_t term = (seq_t)(2 * k + 1) * c1;
        term = term * c2;
        sum += term;
    }
    return sum;
}
