/*
 * A355470  Expansion of Sum_{k>=0} (k^3 * x)^k/(1 - k^3 * x)^(k+1).
 * https://oeis.org/A355470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} k^(3*n) * C(n,k) (sum of k^(3n) times binomial
 *            coefficient)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t exp = 3 * n;          /* exponent 3*n for the power */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = seq_pow((seq_t)k, exp);      /* k^(3*n) */
        term = term * binomial(n, k);             /* multiply by C(n,k) */
        sum += term;                              /* accumulate */
    }
    return sum;
}
