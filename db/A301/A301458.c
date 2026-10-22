/*
 * A301458  a(n) = [x^n] Product_{k>=1} 1/(1 - x^(k*(k+1)/2))^n.
 * https://oeis.org/A301458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} 1/(1 - x^(k*(k+1)/2))^n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate DP and auxiliary arrays */
    seq_t *dp;
    seq_t *M;
#ifdef __cplusplus
    /* C++: use vectors to construct objects */
    std::vector<seq_t> dp_vec(n + 1, (seq_t)0);
    std::vector<seq_t> M_vec(n + 1, (seq_t)0);
    dp = dp_vec.data();
    M = M_vec.data();
#else
    /* C: malloc and zero-initialize (works for plain integer types) */
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    M  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp || !M) {
        /* allocation failed – return zero */
        if (dp) free(dp);
        if (M)  free(M);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
        M[i]  = (seq_t)0;
    }
#endif

    /* Pre‑compute M[j] = sum of triangular numbers that divide j */
    for (idx_t tri = 1; ; ++tri) {
        idx_t T = tri * (tri + 1) / 2;
        if (T > n) break;
        for (idx_t j = T; j <= n; j += T) {
            M[j] += (seq_t)T;
        }
    }

    /* DP recurrence:
       a(k) = (λ/k) * sum_{j=1..k} a(k-j) * M[j],  with λ = n
    */
    dp[0] = (seq_t)1;
    seq_t lambda = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; ++j) {
            sum += dp[k - j] * M[j];
        }
        /* Compute a(k) = sum * λ / k */
        dp[k] = sum * lambda / (seq_t)k;
    }

    seq_t result = dp[n];

#ifndef __cplusplus
    free(dp);
    free(M);
#endif

    return result;
}
