/*
 * A390105  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-2*x/(1-x))
 *          ).
 * https://oeis.org/A390105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390105: a(n) = 2 * n! * Sum_{k=0}^{n-1} (2n+2)^k * binom(n-1, k) / (k+1)! */
/* The implementation evaluates the sum using only integer arithmetic. */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    seq_t multiplier = fact_n;              /* n!/(k+1)! */
    seq_t pow        = 1;                   /* (2n+2)^k   */
    seq_t base       = 2 * (seq_t)(n + 1);  /* 2(n+1)     */
    seq_t binom      = 1;                   /* binom(n-1,k) */
    seq_t sum        = 0;

    for (idx_t k = 0; k < n; ++k) {
        seq_t term = pow * binom * multiplier;
        sum += term;

        if (k < n - 1) {
            pow *= base;

            binom *= (seq_t)(n - 1 - k);
            binom /= (seq_t)(k + 1);

            multiplier /= (seq_t)(k + 2);
        }
    }

    return sum * 2;
}
