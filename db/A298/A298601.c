/*
 * A298601  Expansion of Product_{k>=2} (1 - x^(k^2)).
 * https://oeis.org/A298601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥2} (1 − x^(k^2))
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS A298601.
 * The DP runs in O(n √n) time and uses O(n) space. */
seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1 and initialize to 0. */
    seq_t *coeff = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (coeff == NULL) return 0;            /* allocation failed – return 0 */

    for (idx_t i = 0; i <= n; ++i)
        coeff[i] = 0;
    coeff[0] = 1;                           /* a(0) = 1 */

    /* Process each square k^2 (k ≥ 2) up to n. */
    for (idx_t k = 2; k * k <= n; ++k) {
        idx_t sq = k * k;                   /* square value, fits in idx_t */
        for (idx_t i = n; i >= sq; --i) {
            coeff[i] = coeff[i] - coeff[i - sq];  /* multiply by (1 − x^sq) */
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
