/*
 * A357352  Number of partitions of n into distinct positive triangular
 *          numbers such that the number of parts is a triangular number.
 * https://oeis.org/A357352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 442, u16 n <= 1427, u32 n <= 5962, u64 n <= 8338,
 *            u128 n <= 8338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into distinct triangular numbers with a
 * triangular number of parts.  (OEIS A357352)
 *
 * The implementation uses dynamic programming on the number of parts.
 * For each triangular number t we assemble all partitions of every
 * sum s ≥ t by adding a part t to a partition of s–t that has one
 * fewer part.  After the table is built, we sum the counts where the
 * part count itself is a triangular number.
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case: empty partition of 0 */
    if (n == 0)
        return (seq_t)1;

    /* Generate all triangular numbers ≤ n */
    idx_t triangular[200];
    int triCnt = 0;
    for (int k = 1; ; ++k) {
        idx_t t = (idx_t)k * (k + 1) / 2;
        if (t > n)
            break;
        triangular[triCnt++] = t;
    }

    /* Find the maximal number of distinct triangular parts that can be
     * used such that their sum does not exceed n.  This limits the DP
     * dimension for the part count. */
    idx_t sum = 0;
    int cntmax = 0;
    for (int i = 0; i < triCnt; ++i) {
        sum += triangular[i];
        if (sum <= n)
            ++cntmax;
        else
            break;
    }

    /* DP table: dp[sum][cnt] for 0 ≤ sum ≤ n and 0 ≤ cnt ≤ cntmax.
     * We flatten it into a 1‑D array. */
    size_t stride = (size_t)cntmax + 1;
    size_t tableSize = (size_t)(n + 1) * stride;
    seq_t *dp = (seq_t *)malloc(tableSize * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                 /* allocation failure (unlikely) */
    memset(dp, 0, tableSize * sizeof(seq_t));
    dp[0] = (seq_t)1;                   /* dp[0][0] = 1 */

    /* Main DP loop over distinct triangular numbers. */
    for (int i = 0; i < triCnt; ++i) {
        int t = (int)triangular[i];
        for (int s = (int)n; s >= t; --s) {
            int prevSum = s - t;
            size_t offset      = (size_t)s * stride;
            size_t offsetPrev  = (size_t)prevSum * stride;
            for (int cnt = cntmax; cnt >= 1; --cnt) {
                dp[offset + cnt] += dp[offsetPrev + cnt - 1];
            }
        }
    }

    /* Sum over all counts that are themselves triangular numbers. */
    seq_t result = (seq_t)0;
    for (int i = 1; ; ++i) {
        idx_t tc = (idx_t)i * (i + 1) / 2;
        if (tc > cntmax)
            break;
        result += dp[(size_t)n * stride + (size_t)tc];
    }

    free(dp);
    return result;
}
