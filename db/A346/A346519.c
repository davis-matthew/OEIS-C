/*
 * A346519  Number of partitions of the 2n-multiset {0,...,0,1,2,...,n} into
 *          distinct multisets.
 * https://oeis.org/A346519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346519
   Formula: a(n) = Sum_{j=0..n} Stirling2(n,j) *
              Sum_{i=0..n} binomial(j+i-1,i)*A000009(n-i)
   where A000009(k) is the number of partitions of k into distinct parts. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* calloc, free, malloc, free */

seq_t a(idx_t n)
{
    /* Return a(0) = 1 immediately. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate temporary arrays.  In C we use calloc, in C++ we use new[]. */
    seq_t *dp, *S_prev, *S_cur;
#ifdef __cplusplus
    dp      = new seq_t[(size_t)(n + 1)];
    S_prev  = new seq_t[(size_t)(n + 1)];
    S_cur   = new seq_t[(size_t)(n + 1)];
#else
    dp      = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    S_prev  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    S_cur   = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
#endif

    /* --- 1. A000009: partitions into distinct parts up to n --- */
    dp[0] = (seq_t)1;     /* p_distinct(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
        for (idx_t m = n; m >= k; --m)
            dp[m] += dp[m - k];

    /* --- 2. Stirling numbers of the second kind: S(n,j) --- */
    S_prev[0] = (seq_t)1; /* S(0,0)=1 */
    for (idx_t i = 1; i <= n; ++i) {
        S_cur[0] = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j)
            S_cur[j] = (seq_t)j * S_prev[j] + S_prev[j - 1];
        for (idx_t j = 0; j <= i; ++j)
            S_prev[j] = S_cur[j];
    }

    /* --- 3. Main accumulation --- */
    seq_t total = (seq_t)0;
    for (idx_t j = 1; j <= n; ++j) {      /* number of non‑zero parts */
        seq_t inner  = (seq_t)0;
        seq_t binom  = (seq_t)1;          /* binomial(j-1,0)=1 */
        for (idx_t i = 0; i <= n; ++i) {
            inner += binom * dp[n - i];
            if (i < n) {
                /*   binomial(j+i, i+1) = binomial(j+i-1, i) * (j+i) / (i+1)  */
                seq_t numer   = (seq_t)(j + i);
                seq_t denom   = (seq_t)(i + 1);
                binom = (binom * numer) / denom;
            }
        }
        total += S_prev[j] * inner;
    }

    /* --- 4. Clean up --- */
#ifdef __cplusplus
    delete [] dp;
    delete [] S_prev;
    delete [] S_cur;
#else
    free(dp);
    free(S_prev);
    free(S_cur);
#endif

    return total;
}
