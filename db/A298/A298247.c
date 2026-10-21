/*
 * A298247  Expansion of Product_{k>=1} (1 - x^(k*(k+1)*(k+2)/6)).
 * https://oeis.org/A298247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^{k*(k+1)*(k+2)/6})
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array for coefficients up to n */
    seq_t *f = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!f) return 0;          /* allocation failed – return 0 as sentinel */

    /* initialise coefficients */
    for (idx_t i = 0; i <= n; ++i) f[i] = 0;
    f[0] = 1;

    /* process each tetrahedral number t_k = k(k+1)(k+2)/6 */
    for (idx_t k = 1; ; ++k) 
    {
        idx_t t = k * (k + 1) * (k + 2) / 6;
        if (t > n) break;

        /* update coefficients in descending order to keep parts distinct */
        for (idx_t i = n; i >= t; --i) {
            f[i] -= f[i - t];
        }
    }

    seq_t result = f[n];
    free(f);
    return result;
}
