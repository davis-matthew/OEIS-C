/*
 * A331922  Number of compositions (ordered partitions) of n into distinct
 *          Lucas numbers (beginning with 1).
 * https://oeis.org/A331922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S ⊆ Lucas numbers, distinct, sum S = n} (|S|)!
 * Range:     u8 n <= 43, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lucas numbers used: 1, 3, 4, 7, 11, 18, 29, 47, 76, 123, 199, ...   */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case: a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Gather Lucas numbers ≤ n, omitting the initial L(0)=2 */
    idx_t lucas[64];
    size_t K = 0;
    seq_t n_seq = (seq_t)n;

    seq_t l_prev = (seq_t)2;          /* L(0) */
    seq_t l_curr = (seq_t)1;          /* L(1) */
    if (l_curr <= n_seq) {
        lucas[K++] = (idx_t)l_curr;  /* L(1) = 1 */
    }
    while (1) {
        seq_t l_next = l_prev + l_curr;   /* L(k) = L(k-1) + L(k-2) */
        if (l_next > n_seq) {
            break;
        }
        lucas[K++] = (idx_t)l_next;       /* Store Lucas number ≤ n */
        l_prev = l_curr;
        l_curr = l_next;
    }

    /* DP table: dp[sum][k] = number of subsets of size k that sum to "sum" */
    size_t rows = (size_t)n + 1;
    size_t cols = K + 1;
    size_t total = rows * cols;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) { /* allocation failure: return 0 as error code */
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;  /* base: empty subset sums to 0 */

    /* Update DP for each Lucas number */
    for (size_t li = 0; li < K; ++li) {
        idx_t l = lucas[li];
        for (idx_t sum = n; sum >= l; --sum) {
            for (idx_t k = cols; k--; ) {      /* k goes K-1 .. 0 */
                seq_t add = dp[(size_t)(sum - l) * cols + k];
                if (add) {
                    dp[(size_t)sum * cols + k + 1] += add;
                }
            }
        }
    }

    /* Precompute factorials up to K */
    seq_t fact[65];
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Sum over all subset sizes */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= K; ++k) {
        seq_t ways = dp[(size_t)n * cols + k];
        if (ways) {
            result += ways * fact[k];
        }
    }

    free(dp);
    return result;
}
