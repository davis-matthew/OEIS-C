/*
 * A278380  Number of nX3 0..1 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly three mistakes.
 * https://oeis.org/A278380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 37, u128 n <= 202,
 *            big any n
 * Helpers:   bits_from_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×3 0..1 arrays whose rows and columns
 * are in lexicographic nondecreasing order with exactly three
 * violations (mistakes).
 *
 * A mistake occurs when a row is lexicographically larger than the
 * following row, or when the lexicographic order of any pair of
 * columns is violated.  The function computes this value for any
 * n ≥ 1.
 */
#include <stdbool.h> /* for bool */
#include "../utils/bits_from_row.h"

#define MAX_S 3       /* 0 = equal so far, 1 = first column < second, 2 = first column > second */
#define MAX_M 4       /* count of mistakes, we only keep counts 0..3 */

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* dp[2][8][MAX_S][MAX_S][MAX_M] */
    static seq_t dp[2][8][MAX_S][MAX_S][MAX_M];
    int cur = 0, nxt = 1;

    /* clear dp array */
    for (int i = 0; i < 2; ++i)
        for (int r = 0; r < 8; ++r)
            for (int s1 = 0; s1 < MAX_S; ++s1)
                for (int s2 = 0; s2 < MAX_S; ++s2)
                    for (int m = 0; m < MAX_M; ++m)
                        dp[i][r][s1][s2][m] = 0;

    /* ---------- initialize after first row ----------
     * For every possible first row, compute its column
     * relations and any column mistakes that arise.
     */
    for (int r = 0; r < 8; ++r) {
        int a, b, c;
        bits_from_row(r, &a, &b, &c);

        int s1 = 0, s2 = 0;
        int mist = 0;
        if (a > b) { s1 = 2; mist += 1; }
        else if (a < b) { s1 = 1; }
        if (b > c) { s2 = 2; mist += 1; }
        else if (b < c) { s2 = 1; }

        dp[cur][r][s1][s2][mist] = 1;
    }

    /* ---------- add rows 2 .. n ----------
     * For each subsequent row we propagate all states.
     */
    for (idx_t row_idx = 2; row_idx <= n; ++row_idx) {
        /* clear next layer */
        for (int r = 0; r < 8; ++r)
            for (int s1 = 0; s1 < MAX_S; ++s1)
                for (int s2 = 0; s2 < MAX_S; ++s2)
                    for (int m = 0; m < MAX_M; ++m)
                        dp[nxt][r][s1][s2][m] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int s1 = 0; s1 < MAX_S; ++s1) {
                for (int s2 = 0; s2 < MAX_S; ++s2) {
                    for (int m = 0; m < MAX_M; ++m) {
                        seq_t cnt = dp[cur][prev][s1][s2][m];
                        if (cnt == 0) continue;

                        for (int cur_row = 0; cur_row < 8; ++cur_row) {
                            int a, b, c;
                            bits_from_row(cur_row, &a, &b, &c);

                            /* update column 1–2 state */
                            int ns1 = s1;
                            int col12_mist = 0;
                            if (s1 == 0) {
                                if (a > b) { ns1 = 2; col12_mist = 1; }
                                else if (a < b) ns1 = 1;
                            }

                            /* update column 2–3 state */
                            int ns2 = s2;
                            int col23_mist = 0;
                            if (s2 == 0) {
                                if (b > c) { ns2 = 2; col23_mist = 1; }
                                else if (b < c) ns2 = 1;
                            }

                            /* row mistake: previous row > current row */
                            int row_mist = (prev > cur_row) ? 1 : 0;

                            int new_mist = m + col12_mist + col23_mist + row_mist;
                            if (new_mist >= MAX_M) continue;

                            dp[nxt][cur_row][ns1][ns2][new_mist] += cnt;
                        }
                    }
                }
            }
        }

        /* swap layers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* ---------- sum results with exactly three mistakes ----------
     */
    seq_t result = 0;
    for (int r = 0; r < 8; ++r)
        for (int s1 = 0; s1 < MAX_S; ++s1)
            for (int s2 = 0; s2 < MAX_S; ++s2)
                result += dp[cur][r][s1][s2][3];

    return result;
}
