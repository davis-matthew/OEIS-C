/*
 * A340473  a(n) = n! [x^n] W(-W(x))/(-W(x)), where W(x) is the Lambert W
 *          function.
 * https://oeis.org/A340473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = Sum_{k=0..n-1} (-n)^(n-1-k) * (k+2)^k * binomial(n-1,k)
 *            * a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                    /* for uint64_t, if needed by the build */
#include "../utils/binom.h"            /* for binom() */
#include "../utils/pow_seq.h"          /* for pow_seq() */

/* Computes OEIS sequence A340473 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* m = n-1 (unsigned-safe because n != 0) */
    idx_t m = n - 1;

    /* Precompute the signed base: -n as seq_t */
    seq_t neg_n = -(seq_t)n;

    seq_t sum   = (seq_t)0;
    seq_t pow_neg = (seq_t)1;          /* (-n)^j, starts with j=0 */

    /* Loop over j = 0 .. m, where k = m - j */
    for (idx_t j = 0; j <= m; ++j)
    {
        idx_t k = m - j;            /* k runs from m down to 0 */

        /* binomial coefficient C(m, k) */
        seq_t comb = binom(m, k);

        /* (k+2)^k  (k fits in int for n ≤ ~10000) */
        seq_t pow_k = pow_seq(k + 2, (int)k);

        /* term = comb * pow_k * (-n)^j */
        seq_t term = comb * pow_k * pow_neg;

        sum += term;

        /* Update (-n)^j for next j+1 */
        pow_neg *= neg_n;
    }

    return sum;
}
