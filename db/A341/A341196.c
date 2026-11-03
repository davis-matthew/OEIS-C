/*
 * A341196  a(n) = Sum_{k=0..n} k^4 * (n-k)! * binomial(n,k)^2.
 * https://oeis.org/A341196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} k^4 * (n-k)! * binomial(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
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
    seq_t sum   = 0;                /* accumulated result          */
    seq_t fact  = factorial(n);     /* (n-k)! when k == 0         */
    seq_t binom = 1;                /* binomial(n,0)              */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* k^4 */
        seq_t kseq = (seq_t)k;
        seq_t k2   = kseq * kseq;
        seq_t k4   = k2 * k2;

        /* term = k^4 * fact * binom^2 */
        seq_t term = k4 * fact;
        term      = term * binom;
        term      = term * binom;

        sum += term;

        if (k < n)
        {
            /* fact = (n-k-1)! from (n-k)! */
            fact /= (seq_t)(n - k);

            /* binom = C(n,k+1) from C(n,k) */
            binom *= (seq_t)(n - k);
            binom /= (seq_t)(k + 1);
        }
    }
    return sum;
}
