/*
 * A320964  a(n) = Sum_{j=0..n} Sum_{k=0..j} Stirling2(j - k, k).
 * https://oeis.org/A320964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} Sum_{k=0..j} Stirling2(j - k, k)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on computing all relevant Stirling numbers and
   accumulating the diagonal sums D[n] = Sum_{k} S(n - k, k).  Then
   a(n) = Sum_{i=0..n} D[i]. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for Stirling numbers and diagonal sums. */
    size_t N = (size_t)n;
    seq_t *S = (seq_t *)calloc(N + 2, sizeof(seq_t));   /* current row */
    seq_t *T = (seq_t *)calloc(N + 2, sizeof(seq_t));   /* previous row */
    seq_t *D = (seq_t *)calloc(N + 1, sizeof(seq_t));   /* diagonal sums */

    /* Row 0: S(0,0) = 1 */
    T[0] = (seq_t)1;
    D[0] = (seq_t)1;   /* contributes to a(0) */

    for (idx_t m = 1; m <= n; ++m) {
        /* Compute S(m, k) for k = 0 .. m */
        S[0] = (seq_t)0;                 /* S(m,0) = 0 for m > 0 */
        for (idx_t k = 1; k < m; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            S[k] = (seq_t)k * T[k] + T[k - 1];
        }
        S[m] = (seq_t)1;                     /* S(m,m) = 1 */

        /* Add contributions to D[n] where n = m + k. */
        for (idx_t k = 0; k <= m; ++k) {
            idx_t idx = m + k;
            if (idx <= n)                /* we only need D up to n */
                D[idx] += S[k];
        }

        /* Prepare for next iteration: swap pointers. */
        seq_t *tmp = T;
        T = S;
        S = tmp;
    }

    /* Compute the final sum a(n) = Sum_{i=0..n} D[i]. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i)
        result += D[i];

    /* Memory is not freed to keep the function lightweight; the OS
       reclaims it when the program terminates. */
    return result;
}
