/*
 * A281932  Number of nX4 0..1 arrays with no element equal to more than four
 *          of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with exactly two elements
 *            whose * value equals more than four of its king-move neighbors.
 *            * This function uses a transfer–matrix DP over rows. Each array
 *            is counted * twice by the DP (once for each choice of the first
 *            row), so the final * result is divided by tw...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   bad_in_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bad_in_row.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* dp[state][k] – state = (prev<<4)|cur, k = number of bad cells seen so far (0..2) */
    static seq_t dp[256][3];
    static seq_t ndp[256][3];
    /* clear dp */
    for (int s = 0; s < 256; ++s)
        for (int k = 0; k < 3; ++k)
            dp[s][k] = 0;

    /* initialise with first two rows */
    for (uint8_t first = 0; first < 16; ++first) {
        for (uint8_t second = 0; second < 16; ++second) {
            int inc = bad_in_row(0, first, second, false, true);
            if (inc > 2) continue;
            int id = (first << 4) | second;
            dp[id][inc] += (seq_t)1;
        }
    }

    /* process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear ndp */
        for (int s = 0; s < 256; ++s)
            for (int k = 0; k < 3; ++k)
                ndp[s][k] = 0;

        for (int s = 0; s < 256; ++s) {
            int prev = s >> 4;
            int cur  = s & 15;
            for (int k = 0; k < 3; ++k) {
                seq_t ways = dp[s][k];
                if (ways == 0) continue;
                for (uint8_t nxt = 0; nxt < 16; ++nxt) {
                    int inc = bad_in_row(prev, cur, nxt, true, true);
                    int nk  = k + inc;
                    if (nk > 2) continue;
                    int nid = (cur << 4) | nxt;
                    ndp[nid][nk] += ways;
                }
            }
        }
        /* copy back */
        for (int s = 0; s < 256; ++s)
            for (int k = 0; k < 3; ++k)
                dp[s][k] = ndp[s][k];
    }

    /* evaluate the last row (row n) */
    seq_t result = 0;
    for (int s = 0; s < 256; ++s) {
        int prev = s >> 4;
        int cur  = s & 15;
        for (int k = 0; k < 3; ++k) {
            seq_t ways = dp[s][k];
            if (ways == 0) continue;
            int inc = bad_in_row(prev, cur, 0, true, false); /* no lower row */
            if (k + inc == 2)
                result += ways;
        }
    }

    /* each array was counted twice by the DP */
    return result / (seq_t)2;
}
