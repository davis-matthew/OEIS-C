/*
 * A361093  E.g.f. A(x) satisfies A(x) = exp( 1/(1 - x * A(x)^2) - 1 ).
 * https://oeis.org/A361093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{n-1} n!/(j+1)! * C(n-1,j) * (2n+1)^j, offset 0
 *            (a(0)=1). This implements the equivalent of the e.g.f.
 *            definition A(x)=exp(1/(1-x·A(x)^2)-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 313 (2026-11-01)
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
    /* a(0) is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = (seq_t)0;
    seq_t base  = (seq_t)(2 * n + 1);   /* 2n+1 */
    seq_t power = (seq_t)1;             /* (2n+1)^j */
    seq_t binom = (seq_t)1;             /* C(n-1,0) */
    seq_t ratio = factorial(n);         /* n!/(j+1)! for j=0 */

    for (idx_t j = 0; j < n; ++j) {
        /* term_j = ratio * binom * power */
        seq_t term = ratio * binom;
        term *= power;
        sum += term;

        /* Prepare values for the next j, unless this was the last iteration */
        if (j < n - 1) {
            /* Update binomial coefficient: C(n-1,j+1) = C(n-1,j) * (n-1-j)/(j+1) */
            binom = binom * (seq_t)(n - 1 - j) / (seq_t)(j + 1);

            /* Next power of (2n+1) */
            power *= base;

            /* Next ratio: n!/(j+2)! = (n!/(j+1)!) / (j+2) */
            ratio /= (seq_t)(j + 2);
        }
    }

    return sum;
}
