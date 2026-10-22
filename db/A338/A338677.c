/*
 * A338677  Expansion of Product_{k>=1} 1 / (1 - 8^(k-1)*x^k).
 * https://oeis.org/A338677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(n,k) * 8^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coefficients of the expansion of ∏_{k≥1} 1/(1-8^(k-1)x^k) */
/* for n ≥ 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;
    static size_t coeff_len = 0;

    size_t needed_len = (size_t)n + 1;
    if (needed_len > coeff_len) {
        seq_t *new_coeff = (seq_t *)calloc(needed_len, sizeof(seq_t));
        if (!new_coeff) return 0;             /* allocation failure, return 0 */
        new_coeff[0] = 1;                     /* a(0) = 1 */

        seq_t w = 1;                           /* w = 8^(k-1) for current k */
        for (size_t k = 1; k <= n; ++k) {
            for (size_t m = k; m <= n; ++m) {
                new_coeff[m] += new_coeff[m - k] * w;
            }
            w = w * (seq_t)8;                  /* next power of 8 */
        }

        free(coeff);
        coeff     = new_coeff;
        coeff_len = needed_len;
    }

    return coeff[n];
}
