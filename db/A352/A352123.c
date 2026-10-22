/*
 * A352123  Expansion of e.g.f. (2 - exp(-4*x))^(1/4).
 * https://oeis.org/A352123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-4)^(n-k) * (Product_{j=0}^{k-1} (-4*j+1))
 *            * Stirling2(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for Stirling numbers S(k) and products P(k) */
    seq_t *S = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *P = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!S || !P) {
        /* In the unlikely event of allocation failure, return 0. */
        free(S);
        free(P);
        return (seq_t)0;
    }

    /* P[0] = 1; P[k] = P[k-1] * (1 - 4*(k-1)) for k >= 1 */
    P[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)1 - (seq_t)4 * (k - 1);
        P[k] = P[k - 1] * factor;
    }

    /* Stirling numbers of the second kind: S[0] = 1 for n = 0 */
    S[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Update in descending order to avoid overwrite */
        for (idx_t k = i; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = (seq_t)0;
    }

    /* Compute a(n) = sum_{k=0..n} S(k) * P(k) * (-4)^(n-k) */
    seq_t sum = (seq_t)0;
    seq_t pow_factor = (seq_t)1;                /* (-4)^(0) for k = n */
    for (idx_t k = n; ; --k) {
        seq_t term = S[k] * P[k];               /* S(k) * P(k) */
        term = term * pow_factor;               /* * (-4)^(n-k) */
        sum += term;
        if (k == 0) break;
        pow_factor = pow_factor * (seq_t)-4;    /* next power for k-1 */
    }

    free(S);
    free(P);
    return sum;
}
