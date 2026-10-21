/*
 * A265974  Expansion of Product_{k>=1} 1/(1 - 3*k*x^k).
 * https://oeis.org/A265974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 - 3*k*x^k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate space for coefficients up to n */
    seq_t *coef = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!coef) return (seq_t)0;          /* allocation failure – unlikely in tests */

    /* initialize all coefficients to zero, except a(0)=1 */
    for (idx_t i = 0; i <= n; ++i) coef[i] = 0;
    coef[0] = 1;

    /* iterative multiplication of the infinite series
       (1 + 3k x^k + (3k)^2 x^{2k} + ...) for each k */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)(3 * k);   /* small integer multiplier */

        for (idx_t m = k; m <= n; ++m) {
            /* coef[m] += factor * coef[m - k] */
            coef[m] = coef[m] + factor * coef[m - k];
        }
    }

    seq_t result = coef[n];
    free(coef);
    return result;
}
