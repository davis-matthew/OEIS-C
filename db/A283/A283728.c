/*
 * A283728  Number of 3Xn 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   count_violations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of 3×n binary arrays (entries 0 or 1) in which
 *  every 1 has at most two horizontal, diagonal or antidiagonal 1‑neighbours,
 *  except for exactly one 1 that may have more than two such neighbours.
 *  The sequence starts with a(1)=0, a(2)=6, a(3)=159, … (OEIS A283728). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_violations.h"

seq_t a(idx_t n)
{
    /* DP state: dp[prev2][prev1][v] – number of ways for processed
     * columns up to the current column, where `prev1` is the last
     * processed column, `prev2` is the one before that,
     * and `v` is 0 or 1 (the total count of violating cells seen so far
     * – the last column `prev1` is still pending, its violation will be
     * evaluated only when a new column is added). */
    seq_t dp[8][8][2] = {0}, ndp[8][8][2] = {0};

    /* Initialisation: we have processed the first column only.
     * The virtual column to its left is zeros (prev2 = 0). */
    for (unsigned int bits = 0; bits < 8; ++bits)
        dp[0][bits][0] = (seq_t)1;

    /* Add columns 1 … n-1 (total n-1 additions). */
    for (idx_t step = 0; step < n - 1; ++step) {
        /* reset ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j][0] = ndp[i][j][1] = (seq_t)0;

        for (int prev2 = 0; prev2 < 8; ++prev2) {
            for (int prev1 = 0; prev1 < 8; ++prev1) {
                for (int v = 0; v < 2; ++v) {
                    seq_t cnt = dp[prev2][prev1][v];
                    if (cnt == (seq_t)0)
                        continue;

                    for (int cur = 0; cur < 8; ++cur) {
                        int addv = count_violations(prev2, prev1, cur);
                        if (addv > 1)
                            continue;                 /* more than 1 violation in this column */

                        int nv = v + addv;
                        if (nv > 1)
                            continue;                 /* overall more than 1 violation */

                        ndp[prev1][cur][nv] += cnt;
                    }
                }
            }
        }

        /* move to next state */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j][0] = ndp[i][j][0],
                dp[i][j][1] = ndp[i][j][1];
    }

    /* Final evaluation of the last column with a dummy right column of zeros. */
    seq_t result = (seq_t)0;
    for (int prev2 = 0; prev2 < 8; ++prev2) {
        for (int prev1 = 0; prev1 < 8; ++prev1) {
            for (int v = 0; v < 2; ++v) {
                seq_t cnt = dp[prev2][prev1][v];
                if (cnt == (seq_t)0)
                    continue;

                int addv = count_violations(prev2, prev1, 0);   /* right neighbor is zero */
                if (v + addv == 1)
                    result += cnt;
            }
        }
    }

    return result;
}
