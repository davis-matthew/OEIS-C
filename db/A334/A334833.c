/*
 * A334833  Total length squared of longest runs of 1's in all bitstrings of
 *          length n.
 * https://oeis.org/A334833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{X∈{0,1}^n} (L(X))^2, computed as * a(n) = 2^n * n^2 -
 *            Σ_{k=0}^{n-1} (2k+1) * g(n,k), * where g(n,k) is the number of
 *            n‑bit strings whose longest * run of consecutive 1's is at most
 *            k
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* offset starts at 1 */

    /* Pre‑compute powers of two up to 2^n. */
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow2)
        return 0;
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] + pow2[i - 1];     /* double */

    /* Temporary array for g(*,k). Reused for all k. */
    seq_t *g = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!g) {
        free(pow2);
        return 0;
    }

    seq_t sumTerm = 0;                           /* Σ (2k+1)·g(n,k)   */

    for (idx_t k = 0; k < n; ++k) {
        /* Base case: for i ≤ k, g(i,k) = 2^i. */
        for (idx_t i = 0; i <= k; ++i)
            g[i] = pow2[i];

        /* Sliding window sum for the recurrence g(m,k) = Σ_{j=1}^{k+1} g(m-j,k). */
        seq_t sumWindow = 0;
        for (idx_t i = 0; i <= k; ++i)
            sumWindow += g[i];

        /* Recurrence for i = k+1 … n. */
        for (idx_t i = k + 1; i <= n; ++i) {
            g[i] = sumWindow;
            sumWindow += g[i];
            sumWindow -= g[i - (k + 1)];          /* slide the window */
        }

        seq_t gn   = g[n];
        seq_t term = gn * ((seq_t)(2 * k + 1));   /* (2k+1)·g(n,k) */
        sumTerm += term;
    }

    seq_t nsq   = (seq_t)n * (seq_t)n;           /* n² */
    seq_t result = pow2[n] * nsq - sumTerm;      /* 2^n·n² – Σ(2k+1)·g(n,k) */

    free(pow2);
    free(g);
    return result;
}
