/*
 * A380647  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-3*x)/(1 +
 *          x)^3 ).
 * https://oeis.org/A380647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * n! * Sum_{k=0..n} (3*n+3)^(k-1) * binomial(3*n+3,
 *            n-k) / k! This implementation evaluates the sum directly,
 *            avoiding floating‑point arithmetic and exploiting integer
 *            arithmetic to keep all intermediate values integral
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Base value: 3*n + 3 (fits comfortably in idx_t) */
    seq_t base = (seq_t)((seq_t)3 * n + (seq_t)3);

    /* n! */
    seq_t fact_n = factorial(n);

    /* Sum of terms */
    seq_t sum = 0;

    /* Term for k = 0: 3 * n! * binom(base, n) / base */
    {
        seq_t binom_val = binomial((idx_t)base, n);
        seq_t num = (seq_t)3 * fact_n;          /* (3 * n!) */
        num = num * binom_val;                  /* 3*n!*C(base, n) */
        sum += num / base;                      /* exact division */
    }

    /* Variables for k >= 1 loop */
    seq_t denom  = 1;       /* k! as seq_t, starting with 1! = 1 */
    seq_t pow_val = 1;      /* base^(k-1), initialized for k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update factorial denominator: k! */
        denom *= (seq_t)k;                      /* denom = k! */

        /* n! / k! as integer */
        seq_t nf_over_k = fact_n / denom;

        /* C(base, n-k) */
        seq_t binom_val = binomial((idx_t)base,
                                   (idx_t)(n - k));

        /* Compute term: 3 * (n! / k!) * base^(k-1) * binom_val */
        seq_t term = nf_over_k * pow_val;
        term = term * binom_val;
        term = term * (seq_t)3;

        sum += term;

        /* Prepare pow_val for next iteration: base^k */
        pow_val *= base;
    }

    return sum;
}
