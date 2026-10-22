/*
 * A326506  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly five 0's.
 * https://oeis.org/A326506
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary max‑heaps with n keys from {0,1} that
 *            contain exactly five 0's. Computation uses a dynamic program
 *            over the fixed binary heap tree: we count the number of ways to
 *            select a collection of disjoint subtrees whose sizes sum to 5.
 *            The root is forced to be 1 for n > 5; for n = 5...
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 442, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t K = 5;                       /* we only need up to 5 zeros */
    if (n < K) return 0;                    /* behavior below the offset is irrelevant */
    if (n == K) return (seq_t)1;            /* the all‑zero heap */

    /* precompute subtree sizes */
    idx_t *subsize = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    for (idx_t i = n; (idx_t)i >= 1; --i) {
        idx_t left  = i * 2;
        idx_t right = i * 2 + 1;
        subsize[i] = 1;
        if (left <= n)  subsize[i] += subsize[left];
        if (right <= n) subsize[i] += subsize[right];
    }

    /* DP arrays: dp[i][k] = number of ways in subtree i (root not chosen)
       to obtain exactly k zeros in that subtree. */
    seq_t (*dp)[K + 1] = (seq_t (*)[K + 1])malloc((n + 1) * sizeof(seq_t [K + 1]));
    for (idx_t i = n; (idx_t)i >= 1; --i) {
        for (idx_t k = 0; k <= K; ++k) dp[i][k] = (seq_t)0;

        /* start with no zeros chosen yet */
        seq_t cur[K + 1];
        for (idx_t k = 0; k <= K; ++k) cur[k] = k == 0 ? (seq_t)1 : (seq_t)0;

        /* iterate over children */
        idx_t child[2] = { i * 2, i * 2 + 1 };
        for (int ci = 0; ci < 2; ++ci) {
            idx_t c = child[ci];
            if (c > n) continue;                /* no such child */

            seq_t newdp[K + 1];
            for (idx_t k = 0; k <= K; ++k) newdp[k] = (seq_t)0;

            for (idx_t s = 0; s <= K; ++s) {
                if (cur[s] == (seq_t)0) continue;
                /* option: do not choose a root in child c */
                for (idx_t t = 0; t + s <= K; ++t) {
                    if (dp[c][t] == (seq_t)0) continue;
                    newdp[s + t] += cur[s] * dp[c][t];
                }
                /* option: choose the whole subtree rooted at c as a zero‑subtree */
                if (subsize[c] <= K - s) {
                    newdp[s + subsize[c]] += cur[s];
                }
            }
            /* copy newdp into cur for next child */
            for (idx_t k = 0; k <= K; ++k) cur[k] = newdp[k];
        }
        /* store final dp for node i */
        for (idx_t k = 0; k <= K; ++k) dp[i][k] = cur[k];
    }

    seq_t result = dp[1][K];
    free(dp);
    free(subsize);
    return result;
}
