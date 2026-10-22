/*
 * A336979  Expansion of Product_{k>=1} (1 + x^k * (k + x)).
 * https://oeis.org/A336979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 56, u64 n <= 154, u128 n <= 443,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336979: Expansion of ∏_{k≥1} (1 + x^k (k + x)) */
/* The coefficient of x^n in the expansion is returned. */
/* Implements a direct DP: for each k (1 ≤ k ≤ n) the factor
   (1 + k x^k + x^{k+1}) is multiplied into the current polynomial. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;       /* a(0) = 1 */

    /* Allocate coefficient array a[0..n] */
    size_t N = (size_t)n + 1;
    seq_t *coef = (seq_t *)calloc(N, sizeof(seq_t));
    if (!coef) return 0;               /* allocation failed – return 0 */

    coef[0] = (seq_t)1;                /* start with polynomial '1' */

    /* Multiply successively by each factor (1 + k x^k + x^{k+1}) */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t mult = (seq_t)k;         /* weight of the k‑term */

        /* Update coefficients from high to low to use old values */
        for (idx_t i = n; i >= k; --i) {
            seq_t add = mult * coef[i - k];          /* k x^k contribution */
            if (i >= k + 1) add += coef[i - (k + 1)];/* x^{k+1} contribution */
            coef[i] += add;
        }
    }

    seq_t result = coef[n];
    free(coef);
    return result;
}
