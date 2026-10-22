/*
 * A360987  E.g.f. A(x) satisfies A(x) = exp(x * A(-x)^2).
 * https://oeis.org/A360987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360987
   Formula: a(0) = 1
   a(n) = Sum_{k=1..n} (2*n-2*k+1)^(k-1) * (-2*k)^(n-k) * binomial(n,k)   (for n ≥ 1)
   (The k = 0 term is zero for n > 0 and equals 1 when n = 0.) */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = 0;                         /* initialise accumulator */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial coefficient */
        seq_t bin = binomial(n, k);

        /* (2*n - 2*k + 1)^(k-1) */
        seq_t base1 = (seq_t)( (int64_t)2 * (int64_t)n
                               - (int64_t)2 * (int64_t)k
                               + 1 );
        seq_t pow1  = big_pow(base1, k - 1);

        /* (-2*k)^(n-k) */
        seq_t base2 = (seq_t)( -(int64_t)2 * (int64_t)k );
        seq_t pow2  = big_pow(base2, n - k);

        /* term = bin * pow1 * pow2 */
        seq_t term = bin * pow1 * pow2;
        sum += term;
    }

    return sum;
}
