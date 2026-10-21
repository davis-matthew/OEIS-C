/*
 * A251784  Number of (n+2)X(1+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 2 3 6 or 7.
 * https://oeis.org/A251784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 51, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A251784: number of (n+2)×3 arrays of digits 0–3 with no
 * 3×3 subblock containing a row, column, main‑diagonal or anti‑diagonal
 * whose sum equals 2, 3, 6 or 7.
 *
 * The algorithm uses dynamic programming on the rows of the array.
 * There are 4³ = 64 possible row patterns.  A pattern is “valid” if its
 * row sum is not 2,3,6,7.  For each pair of consecutive rows we precompute
 * the list of third rows that together with the pair form a legal
 * 3×3 subblock.  The DP then iterates over the n+2 rows adding one by
 * one, accumulating the count in a 64×64 table.  Complexity is
 * O(n·64²) and uses only a few kilobytes of memory. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* --- lazy static initialisation of all tables -------------------- */
    static bool initialized = false;
    static uint8_t  validRow[64];
    static uint8_t  rowDigits[64][3];
    static seq_t   rowSum[64];
    static uint8_t nextLen[64][64];
    static uint8_t nextList[64][64][64];      /* maximum 64 entries per pair */
    static seq_t   dp[64][64];
    static seq_t   dp_next[64][64];

    if (!initialized) {
        /* forbidden sums */
        bool badSum[13] = {false};
        badSum[2] = badSum[3] = badSum[6] = badSum[7] = true;

        /* pre‑compute row digits and legal rows */
        for (int i = 0; i < 64; ++i) {
            int tmp = i;
            for (int d = 0; d < 3; ++d) {
                rowDigits[i][d] = tmp & 3;   /* base‑4 least significant digit */
                tmp >>= 2;                   /* divide by 4 */
            }
            rowSum[i]   = (seq_t)rowDigits[i][0] +
                          (seq_t)rowDigits[i][1] +
                          (seq_t)rowDigits[i][2];
            validRow[i] = !badSum[(int)rowSum[i]];
        }

        /* pre‑compute legal third rows for each pair (a,b) */
        for (int a = 0; a < 64; ++a) {
            if (!validRow[a]) continue;
            for (int b = 0; b < 64; ++b) {
                if (!validRow[b]) {
                    nextLen[a][b] = 0;
                    continue;
                }
                int len = 0;
                for (int r = 0; r < 64; ++r) {
                    if (!validRow[r]) continue;

                    int c0 = rowDigits[a][0] + rowDigits[b][0] + rowDigits[r][0];
                    int c1 = rowDigits[a][1] + rowDigits[b][1] + rowDigits[r][1];
                    int c2 = rowDigits[a][2] + rowDigits[b][2] + rowDigits[r][2];
                    if (badSum[c0] || badSum[c1] || badSum[c2]) continue;

                    int md = rowDigits[a][0] + rowDigits[b][1] + rowDigits[r][2];
                    int ad = rowDigits[a][2] + rowDigits[b][1] + rowDigits[r][0];
                    if (badSum[md] || badSum[ad]) continue;

                    nextList[a][b][len++] = (uint8_t)r;
                }
                nextLen[a][b] = len;
            }
        }
        initialized = true;
    }

    /* ---- base case handling ---------------------------------------- */
    if (n == 0) return 0;                /* undefined for n < 1 */
    const idx_t rows = n + 2;             /* total number of rows */

    /* zero DP tables */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* initial state after two rows (both must be valid rows) */
    for (int a = 0; a < 64; ++a) {
        if (!validRow[a]) continue;
        for (int b = 0; b < 64; ++b)
            if (validRow[b]) dp[a][b] = 1;
    }

    /* DP for rows 3 … rows (inclusive) */
    for (idx_t i = 3; i <= rows; ++i) {
        /* zero next table */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp_next[a][b] = 0;

        /* transitions */
        for (int a = 0; a < 64; ++a) {
            if (!validRow[a]) continue;
            for (int b = 0; b < 64; ++b) {
                if (!validRow[b]) continue;
                seq_t val = dp[a][b];
                if (val == 0) continue;
                const int len = nextLen[a][b];
                const uint8_t *list = nextList[a][b];
                for (int k = 0; k < len; ++k) {
                    int r = list[k];
                    dp_next[b][r] += val;
                }
            }
        }

        /* swap tables for next iteration */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp[a][b] = dp_next[a][b];
    }

    /* accumulate final result */
    seq_t total = 0;
    for (int a = 0; a < 64; ++a) {
        if (!validRow[a]) continue;
        for (int b = 0; b < 64; ++b)
            if (validRow[b]) total += dp[a][b];
    }
    return total;
}
