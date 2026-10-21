/*
 * A263074  Expansion of phi(-x) / (chi(-x^3) * chi(-x^5)) in powers of x
 *          where phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A263074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Prod_{k>=1} (1 + x^(3k)) * (1 +
 *            x^(5k)) * (1 - x^k) / (1 + x^k)
 * Range:     i8 n <= 195, i16 n <= 625, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t N = n;
    if (N == (idx_t)0) return (seq_t)1;   /* a(0) = 1 */

    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!coeff) return (seq_t)0;          /* memory failure protection */

    /* initial series: 1 */
    for (idx_t i = 0; i <= N; ++i)
        coeff[i] = (i == 0) ? (seq_t)1 : (seq_t)0;

    for (idx_t k = 1; k <= N; ++k) {
        idx_t m;

        /* multiply by (1 + x^(3k)) */
        m = 3 * k;
        if (m <= N) {
            for (idx_t i = N; i >= m; --i) {
                coeff[i] += coeff[i - m];
            }
        }

        /* multiply by (1 + x^(5k)) */
        m = 5 * k;
        if (m <= N) {
            for (idx_t i = N; i >= m; --i) {
                coeff[i] += coeff[i - m];
            }
        }

        /* multiply by (1 - x^k) */
        m = k;
        if (m <= N) {
            for (idx_t i = N; i >= m; --i) {
                coeff[i] -= coeff[i - m];
            }
        }

        /* divide by (1 + x^k)   (coefficientwise division) */
        if (k <= N) {
            for (idx_t i = k; i <= N; ++i) {
                coeff[i] -= coeff[i - k];
            }
        }
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}
