/*
 * A360001  Expansion of Product_{k>=0} (1 - x^(k^2+4)) in powers of x.
 * https://oeis.org/A360001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   Expansion of Product_{k>=0} (1 - x^(k^2+4)) in powers of x
 * Range:     i8 n <= 819, i16 n <= 3373, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) where a(n) is the coefficient of x^n in
   ∏_{k≥0} (1 - x^(k^2+4)). */
seq_t a(idx_t n)
{
    /* Allocate array for coefficients up to n. */
    seq_t *coef = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!coef)
        return 0;           /* out‑of‑memory: return zero (should never happen) */

    /* Initialize all coefficients to 0 except a[0] = 1. */
    for (idx_t i = 0; i <= n; ++i)
        coef[i] = 0;
    coef[0] = 1;

    /* Multiply by each factor (1 - x^e) where e = k^2 + 4 ≤ n. */
    for (idx_t k = 0; ; ++k) {
        idx_t e = k * k + 4;
        if (e > n)
            break;

        /* Update coefficients in descending order to avoid overwriting
           values that are needed for later updates. */
        idx_t d = n - e;
        for (idx_t t = 0; t <= d; ++t) {
            idx_t j = n - t;          /* j runs from n down to e */
            coef[j] = coef[j] - coef[j - e];
        }
    }

    seq_t result = coef[n];
    free(coef);
    return result;
}
