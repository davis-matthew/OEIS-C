/*
 * A251168  Number of (n+1)X(1+1) 0..3 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays with entries in 0..3 such that
 *            for every adjacent pair of rows (a,b) and (c,d) we have min(a,d)
 *            ≥ |b‑c|. Computed by dynamic programming on a 16×16 adjacency
 *            matrix of all possible row pairs
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return 0;

    /* Adjacency matrix for the 16 possible row pairs */
    static bool allowed[16][16];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 16; ++i) {
            int a = i / 4;
            int b = i % 4;
            for (int j = 0; j < 16; ++j) {
                int c = j / 4;
                int d = j % 4;
                int diag_min = a < d ? a : d;
                int diff = b > c ? b - c : c - b;
                allowed[i][j] = (diag_min >= diff);
            }
        }
        initialized = true;
    }

    /* Dynamic programming arrays: dp holds counts for current row number,
       ndp holds counts for next row number.  Initially, all row states
       are possible for the first row. */
    seq_t dp[16];
    seq_t ndp[16];
    for (int i = 0; i < 16; ++i) dp[i] = (seq_t)1;

    /* For each transition (adjacent pair of rows). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int j = 0; j < 16; ++j) ndp[j] = (seq_t)0;
        for (int i = 0; i < 16; ++i) {
            if (dp[i] == 0) continue;  /* skip zero contributions */
            seq_t val = dp[i];
            for (int j = 0; j < 16; ++j) {
                if (allowed[i][j]) ndp[j] += val;
            }
        }
        /* Move to next step */
        for (int j = 0; j < 16; ++j) dp[j] = ndp[j];
    }

    /* Sum all ending states: total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 16; ++i) total += dp[i];
    return total;
}
