/*
 * A326447  Sum of the sixth largest parts in the partitions of n into 8
 *          squarefree parts.
 * https://oeis.org/A326447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculation of OEIS sequence A326447:
 *  a(n) = Sum of the 6th largest part (3rd smallest) in each unordered
 *  partition of n into 8 square‑free parts.
 *  The algorithm counts partitions with DP on the parts (sorted ascending)
 *  while accumulating the value of the 3rd part for each partition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* allocate DP tables: dp[k][s] = number of partitions of s into k parts,
     * sum3[k][s] = sum of the 3rd part over those partitions. */
    size_t sz = (size_t)n + 1;
    seq_t **dp    = (seq_t **)malloc(9 * sizeof(seq_t *));
    seq_t **sum3  = (seq_t **)malloc(9 * sizeof(seq_t *));
    for (int k = 0; k <= 8; ++k) {
        dp[k]   = (seq_t *)calloc(sz, sizeof(seq_t));
        sum3[k] = (seq_t *)calloc(sz, sizeof(seq_t));
    }

    /* build list of square‑free numbers up to n */
    bool *is_sqfree = (bool *)calloc(sz, sizeof(bool));
    for (idx_t i = 0; i < sz; ++i) is_sqfree[i] = true;
    for (idx_t p = 2; (p * p) <= n; ++p) {
        idx_t sq = p * p;
        for (idx_t m = sq; m <= n; m += sq)
            is_sqfree[m] = false;
    }

    dp[0][0] = (seq_t)1;          /* empty partition */

    /* DP over each square‑free part value v in ascending order */
    for (idx_t v = 1; v <= n; ++v) {
        if (!is_sqfree[v]) continue;
        seq_t v_seq = (seq_t)v;

        for (int k = 1; k <= 8; ++k) {
            for (idx_t s = v; s <= n; ++s) {
                seq_t cnt = dp[k - 1][s - v];
                if (!cnt) continue;

                dp[k][s] += cnt;

                if (k - 1 >= 3) {
                    /* previously had at least 3 parts, third part unchanged */
                    sum3[k][s] += sum3[k - 1][s - v];
                } else if (k - 1 == 2) {
                    /* new partition now has exactly 3 parts, third part = v */
                    sum3[k][s] += v_seq * cnt;
                } /* k-1 <=1 → no third part yet */
            }
        }
    }

    seq_t result = sum3[8][n];

    /* free allocated memory */
    for (int k = 0; k <= 8; ++k) {
        free(dp[k]);
        free(sum3[k]);
    }
    free(dp);
    free(sum3);
    free(is_sqfree);

    return result;
}
