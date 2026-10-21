/*
 * A295406  a(n) = n! * Laguerre(n, 2*n, -n).
 * https://oeis.org/A295406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(3*n, n-k) * n^k / k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* factorial(n) */
    seq_t fact = factorial(n);

    /* initial term for k=0:
       term0 = binomial(3*n, n) * n! */
    seq_t term = binomial(3 * n, n) * fact;
    seq_t sum  = term;

    /* iterate k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term_k = term_{k-1} * n * (n-(k-1)) /
                    ( k * (2*n + k) )  */
        seq_t numer   = (term * (seq_t)n) * (seq_t)(n - (k - 1));
        seq_t denom   = (seq_t)k * (seq_t)(2 * n + k);
        term = numer / denom;
        sum += term;
    }

    return sum;
}
