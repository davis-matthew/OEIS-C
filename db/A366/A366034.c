/*
 * A366034  G.f. A(x) satisfies A(x) = 1/(1 - x)^3 + x*(1 - x)^3*A(x)^3.
 * https://oeis.org/A366034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+2, n-k) * binomial(3*k, k) /
 *            (2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the multiplicative binomial helper from ../utils/binom.h. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial(n + 2*k + 2, n - k) */
        seq_t term = binom(n + 2 * k + 2, n - k);
        /* binomial(3*k, k) */
        term = term * binom(3 * k, k);
        /* divide by (2*k + 1) – exact integer division */
        term = term / (seq_t)(2 * k + 1);
        result += term;
    }
    return result;
}
