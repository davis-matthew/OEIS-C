/*
 * A277168  Coefficients in the series reversion of x*exp(-x^2).
 * https://oeis.org/A277168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-1) * (2*n-1)^(n-2) * n! (a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binom, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                      /* a(1) = 1 */

    idx_t two_n_minus1 = 2 * n - 1;                   /* 2n-1 */
    /* binomial coefficient C(2n-1, n-1) */
    seq_t comb = binom(two_n_minus1, n - 1);

    /* (2n-1)^(n-2)  (exponent n-2 >= 0 for n>=2) */
    seq_t pow_val = seq_pow((seq_t)two_n_minus1, n - 2);

    /* n! */
    seq_t fact = factorial(n);

    /* product */
    return comb * pow_val * fact;
}
