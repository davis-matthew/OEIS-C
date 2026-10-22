/*
 * A386975  a(n) is the permanent of the n X n matrix M_n with M_n(j,k) = j
 *          for j <> k, M_n(j,k) = n+j for j = k.
 * https://oeis.org/A386975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n×n matrix M_n with * M_n(j,k) = j for j
 *            ≠ k, * M_n(j,j) = n + j. * The permanent can be computed by
 *            dynamic programming: * dp[k] = sum over all subsets S of {1..n}
 *            with |S| = k of * ∏_{j∈S} (j+n) · ∏_{j∉S} j. * Then a(n) =
 *            Σ_{k=0}^n dp[k] · D_{n-k}, where D_m is the numbe...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* The function signature that must be provided */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate two arrays of size n+1 for dp and derangements */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *D  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp || !D) {
        free(dp);
        free(D);
        return (seq_t)0;          /* Allocation failure propagates */
    }

    /* Initialize dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Dynamic programming over rows 1..n */
    for (idx_t j = 1; j <= n; ++j) {
        /* Reverse loop to avoid overwriting needed values */
        for (idx_t f = j; f >= 1; --f) {
            /* dp[f] = dp[f] * j + dp[f-1] * (j + n) */
            seq_t term_fixed   = dp[f] * (seq_t)j;
            seq_t term_notfixed = dp[f - 1] * (seq_t)(j + n);
            dp[f] = term_fixed + term_notfixed;
        }
        /* Update dp[0] (no fixed point in current row) */
        dp[0] = dp[0] * (seq_t)j;
    }

    /* Compute derangements D[0..n] */
    D[0] = (seq_t)1;
    if (n >= 1)
        D[1] = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        /* D[i] = (i-1) * (D[i-1] + D[i-2]) */
        D[i] = (seq_t)(i - 1) * (D[i - 1] + D[i - 2]);
    }

    /* Sum dp[k] * D[n-k] to get the permanent */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        result += dp[k] * D[n - k];
    }

    free(dp);
    free(D);
    return result;
}
