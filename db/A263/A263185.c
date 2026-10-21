/*
 * A263185  E.g.f. is the series S(x) such that C(x) + i*S(x) = 1 + i *
 *          Integral C(x)/(C(x) + i*S(x)) dx, odd powers only, where i^2 = -1.
 * https://oeis.org/A263185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is computed via the recurrence * * Let B[0] = 1 (this
 *            equals a(1)). * For m >= 1: * G[m] = Σ_{i=0}^{m-1} binom(2m,
 *            2i+1) * B[i] * B[m-1-i] * Q = Σ_{k=1}^{m} B[m-k] * G[k] *
 *            binom(2m, 2k) * B[m] = G[m] - 2 * Q * * Then a(n) = B[n-1]. * *
 *            The recurrence follows from the identities * S' = (1+S²...
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the binom utility to compute binomial coefficients.
    It returns a seq_t value. */
#include "../utils/binom.h"
#include <stdlib.h>
#include <stdint.h>

/* External linkage: the build supplies seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* the sequence is defined only for n >= 1 */

    /* Allocate arrays for B and G.  G[0] is unused. */
    seq_t *B = (seq_t *)malloc(n * sizeof(seq_t));
    seq_t *G = (seq_t *)malloc(n * sizeof(seq_t));
    if (!B || !G) return 0;  /* allocation failed */

    B[0] = 1;   /* a(1) = 1 */
    G[0] = 0;   /* unused */

    for (idx_t m = 1; m < n; ++m)
    {
        idx_t twom = 2 * m;          /* 2m as idx_t */

        /* Compute G[m] = Σ_{i=0}^{m-1} binom(2m, 2i+1) * B[i] * B[m-1-i] */
        seq_t Gn = 0;
        for (idx_t i = 0; i < m; ++i)
        {
            seq_t term = binom(twom, 2 * i + 1);
            term *= B[i];
            term *= B[m - 1 - i];
            Gn += term;
        }
        G[m] = Gn;

        /* Compute Q = Σ_{k=1}^{m} B[m-k] * G[k] * binom(2m, 2k) */
        seq_t Q = 0;
        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t term = B[m - k];
            term *= G[k];
            term *= binom(twom, 2 * k);
            Q += term;
        }

        /* B[m] = Gn - 2 * Q */
        B[m] = Gn;
        B[m] -= (seq_t)2 * Q;
    }

    seq_t result = B[n - 1];

    /* Free allocated memory */
    free(B);
    free(G);
    return result;
}
