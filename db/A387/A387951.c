/*
 * A387951  E.g.f. A(x) satisfies A(x) = exp( x^3*A(x)^3 / (1 - x*A(x))^3 ).
 * https://oeis.org/A387951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1}^{⌊n/3⌋} (n+1)^(k-1) * binomial(n-1, n-3k) /
 *            k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 0, a(2) = 0 – handled naturally by the loop (no k). */
    if (n == 0) return (seq_t)1;

    /* Compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum = 0;
    seq_t fact_k = 1;          /* k!  (k = 1 initially, so 1! = 1) */
    seq_t pow_base = 1;        /* (n+1)^(k-1), starts with exponent 0 */
    seq_t base = (seq_t)(n + 1);

    idx_t maxk = n / 3;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t r = n - 3 * k;                     /* r = n - 3k   */
        seq_t binom = binomial(n - 1, r);        /* C(n-1, r)    */

        seq_t tmp = fact_n / fact_k;             /* n! / k! is integer */
        seq_t term = binom * pow_base * tmp;     /* integer term  */

        sum += term;

        /* Prepare for next k */
        fact_k *= (seq_t)(k + 1);      /* (k+1)!       */
        pow_base *= base;              /* (n+1)^k      */
    }

    return sum;
}
