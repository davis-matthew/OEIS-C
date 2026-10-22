/*
 * A377829  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x)/(1 +
 *          x)^2 ).
 * https://oeis.org/A377829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A377829
 *  a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(2*n+2, n-k) / k!
 *  a(0) = 1
 */

#include "../utils/binomial.h"   /* binomial(a,b) -> seq_t */
#include "../utils/factorial.h"  /* factorial(n) -> seq_t */

seq_t a(idx_t n)
{
    /* base factorial n! */
    seq_t fact_n = factorial(n);

    /* initial binomial C(2n+2, n) for k = 0 */
    seq_t binom = binomial(2 * n + 2, n);

    /* ratio = n! / k! ; for k = 0 this is n! itself */
    seq_t ratio = fact_n;

    seq_t result = 0;
    seq_t pow_factor = 1;            /* (n+1)^{k-1} for k = 1  (exponent 0) */

    /* k = 0 term: (n+1)^{-1} factor */
    seq_t term = binom * ratio;
    term /= (seq_t)(n + 1);          /* exact division, term is integer */
    result += term;

    /* prepare for k = 1 (only if n > 0) */
    if (n > 0) {
        binom = binom * (seq_t)(n - 0) / (seq_t)(n + 0 + 3);
        /* ratio stays the same (division by 1) */
    }

    /* k from 1 to n */
    for (idx_t k = 1; k <= n; ++k) {
        term = binom * ratio * pow_factor;
        result += term;

        if (k < n) {
            /* update binomial to C(2n+2, n-(k+1)) */
            binom = binom * (seq_t)(n - k) / (seq_t)(n + k + 3);

            /* update ratio to n!/(k+1)! */
            ratio = ratio / (seq_t)(k + 1);

            /* pow_factor becomes (n+1)^{(k+1)-1} for next iteration */
            pow_factor *= (seq_t)(n + 1);
        }
    }

    return result;
}
