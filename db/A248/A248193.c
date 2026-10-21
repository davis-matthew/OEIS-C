/*
 * A248193  Expansion of Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k)^2 * x^(5*k).
 * https://oeis.org/A248193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} binomial(n-5*k, k)^2 (Expansion of
 *            Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k)^2 * x^(5*k).)
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 49, u64 n <= 94, u128 n <= 183,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Computes A248193: the coefficient of x^n in
   ∑_{m≥0} x^m * ∑_{k=0..m} C(m,k)^2 * x^(5k) */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; 5 * k <= n; ++k)
    {
        seq_t c = binomial(n - 5 * k, k); /* C(n-5k, k) */
        sum += c * c;                     /* square and add */
    }
    return sum;
}
