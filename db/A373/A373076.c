/*
 * A373076  Number of partitions of n such that (smallest part) > 5*(number of
 *          parts).
 * https://oeis.org/A373076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose smallest part > 5×(number
 *            of parts)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the OEIS sequence A373076.
 *  For each k such that 5k^2 + k <= n we add the number of partitions of
 *  n - 5k^2 - k into parts of size at most k.  This equals the number of
 *  partitions of n into exactly k parts all at least 5k+1.
 *
 *  The implementation builds the classical partition table p[i][j] where
 *  p[i][j] counts partitions of j into parts <= i.  Complexity per call
 *  is O(kmax * n) with kmax = floor( (sqrt(20n+1)-1)/10 ), which is
 *  <= 44 for n <= 10000.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the maximum k needed for this n */
    idx_t kmax = 0;
    for (idx_t k = 0; ; ++k) {
        unsigned __int128 val = (unsigned __int128)5 * k * k + k;
        if (val > n) break;
        kmax = k;
    }

    /* Allocate DP table: (kmax+1) rows, (n+1) columns */
    size_t rows = (size_t)kmax + 1;
    size_t cols = (size_t)n + 1;
    size_t size = rows * cols;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure, return 0 */

    /* Initialize row 0: only partition of 0 */
    for (size_t j = 0; j < cols; ++j)
        dp[j] = (j == 0) ? 1 : 0;

    /* DP recurrence: p[i][j] = p[i-1][j] + p[i][j-i] if j>=i */
    for (size_t i = 1; i < rows; ++i) {
        seq_t *prev = dp + (i - 1) * cols;
        seq_t *curr = dp + i * cols;
        for (size_t j = 0; j < cols; ++j) {
            seq_t val = prev[j];
            if (j >= i) val += curr[j - i];
            curr[j] = val;
        }
    }

    /* Sum contributions for all admissible k */
    seq_t result = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        unsigned __int128 m_val = 5 * (unsigned __int128)k * k + k;
        idx_t m = (idx_t)(n - m_val);
        result += dp[k * cols + m];
    }

    free(dp);
    return result;
}
