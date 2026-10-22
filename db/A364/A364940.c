/*
 * A364940  E.g.f. satisfies A(x) = exp( x*A(x) / (1 - x*A(x))^3 ).
 * https://oeis.org/A364940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} (n+1)^{k‑1} * binomial(n+2k-1, n‑k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (For n=0 the unique term is a(0)=1.) */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */

    seq_t n_fact = factorial(n);          /* n!                      */
    seq_t sum    = 0;                      /* running total           */
    seq_t pf     = n_fact;                 /* n! / k! for current k   */
    seq_t pow_n1 = 1;                      /* (n+1)^{k-1} for k=1     */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(n+2k‑1, n‑k)  */
        seq_t bin = binomial(n + 2 * k - 1, n - k);

        /* term = bin * (n+1)^{k‑1} * (n! / k!) */
        seq_t term = bin;
        term = term * pow_n1;
        term = term * pf;
        sum += term;

        /* update for next k   */
        pow_n1 = pow_n1 * (seq_t)(n + 1);  /* (n+1)^k  */
        pf     = pf / (k + 1);            /* n!/(k+1)! */
    }
    return sum;                            /* a(n) */
}
