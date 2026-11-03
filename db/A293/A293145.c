/*
 * A293145  a(n) = n! * [x^n] exp(n*x/(1 - x)).
 * https://oeis.org/A293145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} n^k * binomial(n-1,k-1) / k! (a(0)=1) *
 *            Implements the definition from OEIS A293145
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 326 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return 1;

    /* Factorial of n (used to factor out k! in the summand) */
    seq_t nfact   = factorial(n);

    /* Running products and accumulators */
    seq_t fact_ratio = nfact;          /* n! / k!    (k = current loop index) */
    seq_t pow_n = (seq_t)n;            /* n^k        (k = current loop index) */
    seq_t bin  = 1;                    /* binomial(n-1,k-1) */
    seq_t sum  = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = (n! / k!) * n^k * C(n-1,k-1) */
        seq_t term = fact_ratio * pow_n * bin;
        sum += term;

        /* Prepare for the next iteration (if any) */
        if (k < n)
        {
            pow_n *= (seq_t)n;                         /* n^(k+1)   */
            fact_ratio /= (seq_t)(k + 1);              /* n! / (k+1)! */
            bin = bin * (seq_t)(n - k) / (seq_t)k;     /* C(n-1,k)  */
        }
    }
    return sum;
}
