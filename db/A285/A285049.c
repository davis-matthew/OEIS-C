/*
 * A285049  Expansion of Product_{k>=0} 1/(1-x^(5*k+1))^(5*k+1).
 * https://oeis.org/A285049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 - x^(5k+1))^-(5k+1)
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 107, u64 n <= 271,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over part sizes d ≡ 1 (mod 5). */

#include <stdlib.h>
#include <stdint.h>
/* seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    size_t len = (size_t)n;

    /* Allocate DP arrays of length len+1, zero‑initialized.  */
    seq_t *dp_cur = (seq_t*)calloc(len + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(len + 1, sizeof(seq_t));
    if (!dp_cur || !dp_next) return (seq_t)0;

    dp_cur[0] = (seq_t)1;        /* initial polynomial: 1 */

    /* Iterate over all allowed part sizes d = 5k + 1 */
    for (size_t d = 1; d <= len; d += 5) {
        /* Number of terms in the series for this d */
        size_t maxM = len / d;
        seq_t *vals = (seq_t*)calloc(maxM + 1, sizeof(seq_t));
        if (!vals) { free(dp_cur); free(dp_next); return (seq_t)0; }

        /* Compute binomial coefficients binom(d+m-1, m) */
        vals[0] = (seq_t)1;
        seq_t binom = (seq_t)1;
        for (size_t m = 1; m <= maxM; ++m) {
            binom = binom * (seq_t)(d + m - 1) / (seq_t)m;
            vals[m] = binom;
        }

        /* Zero dp_next */
        for (size_t i = 0; i <= len; ++i) dp_next[i] = (seq_t)0;

        /* Convolution: dp_next = dp_cur * ∑_{m≥0} vals[m] x^{d*m} */
        for (size_t m = 0; m <= maxM; ++m) {
            size_t deg = m * d;
            seq_t coeff = vals[m];
            for (size_t i = 0; i + deg <= len; ++i) {
                dp_next[i + deg] += dp_cur[i] * coeff;
            }
        }

        /* Swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        free(vals);
    }

    seq_t result = dp_cur[len];
    free(dp_cur);
    free(dp_next);
    return result;
}
