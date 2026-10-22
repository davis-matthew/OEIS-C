/*
 * A326510  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly nine 0's.
 * https://oeis.org/A326510
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 104, u64 n <= 1154,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A326510
 *  Number of binary max‑heaps on n elements from the set {0,1}
 *  containing exactly nine 0's.
 *
 *  The algorithm counts the subsets of nodes that form a union of
 *  whole subtrees (closed under descendants).  For a node i:
 *     DP[i][k] = number of ways to obtain k zeros in the subtree rooted
 *                at i.
 *  Recurrence:
 *     DP[i][k] = [k == size(i)]                     // whole subtree zero
 *                + Σ LIS(kL,kR) DP[left][kL]*DP[right][kR]
 *  where the sum runs over all splits kL+kR = k.
 *  The answer is DP[1][9].
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 9. No defined values for n < 9. */
    if (n < 9) return 0;

    const idx_t K = 9;           /* maximum number of zeros to count   */
    const size_t DP_SIZE = (n + 1) * (K + 1);

    /* Allocate DP array: DP[i*(K+1)+k] */
    seq_t *dp = NULL;
#ifdef __cplusplus
    dp = new seq_t[DP_SIZE]();   /* value‑initialisation sets to zero   */
#else
    dp = (seq_t *)calloc(DP_SIZE, sizeof(seq_t));
    if (!dp) return 0;
#endif

    /* Allocate subtree size array. */
    idx_t *subsize = NULL;
#ifdef __cplusplus
    subsize = new idx_t[n + 1];
#else
    subsize = (idx_t *)calloc(n + 1, sizeof(idx_t));
    if (!subsize) {
        free(dp);
        return 0;
    }
#endif

    /* Compute subtree sizes bottom‑up. */
    for (idx_t i = n; i > 0; --i) {
        idx_t left  = i << 1;
        idx_t right = left | 1;
        idx_t sz = 1;
        if (left <= n)  sz += subsize[left];
        if (right <= n) sz += subsize[right];
        subsize[i] = sz;
    }

    /* DP computation bottom‑up. */
    for (idx_t i = n; i > 0; --i) {
        idx_t left  = i << 1;
        idx_t right = left | 1;
        idx_t leftSize  = (left <= n) ? subsize[left]  : 0;
        idx_t rightSize = (right <= n) ? subsize[right] : 0;
        idx_t sz = subsize[i];

        for (idx_t k = 0; k <= K; ++k) {
            seq_t ways = 0;

            /* Whole-subtree zero case. */
            if (k == sz) ways += (seq_t)1;

            /* Split zeros between children. */
            for (idx_t l = 0; l <= k; ++l) {
                seq_t lw = (left <= n) ? dp[left * (K + 1) + l] : ((l == 0) ? (seq_t)1 : (seq_t)0);
                seq_t rw = (right <= n) ? dp[right * (K + 1) + (k - l)] : (((k - l) == 0) ? (seq_t)1 : (seq_t)0);
                ways += lw * rw;
            }

            dp[i * (K + 1) + k] = ways;
        }
    }

    seq_t result = dp[1 * (K + 1) + K];

    /* Free memory. */
#ifdef __cplusplus
    delete[] dp;
    delete[] subsize;
#else
    free(dp);
    free(subsize);
#endif

    return result;
}
