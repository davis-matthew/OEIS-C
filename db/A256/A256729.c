/*
 * A256729  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum zero and no antidiagonal sum three.
 * https://oeis.org/A256729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 *  Sequence A256729
 *
 *  This sequence counts the number of (n+2)×4 binary arrays with no
 *  3×3 subblock whose main diagonal sums to 0 and no 3×3 subblock
 *  whose anti‑diagonal sums to 3.  The values are computed by
 *  dynamic programming over 4‑bit row states, enforcing the forbidden
 *  patterns on every consecutive triple of rows.  The algorithm
 *  runs in O(n) time with a small constant factor and uses only
 *  integer arithmetic on the type seq_t supplied by the build.
 *
 *  Function:
 *      seq_t a(idx_t n)
 */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Number of rows in the array  = n + 2 */
    idx_t m = n + 2;
    if (m < 3) return 0;               /* offset starts at 1, not used */

    /* Pre‑compute allowed triples of rows */
    static bool allowed[16][16][16];
    static bool initialized = false;
    if (!initialized) {
        for (int r1 = 0; r1 < 16; ++r1) {
            for (int r2 = 0; r2 < 16; ++r2) {
                for (int r3 = 0; r3 < 16; ++r3) {
                    bool ok = true;
                    for (int j = 0; j <= 1 && ok; ++j) {
                        /* Main diagonal sum, must not be zero */
                        int msum = ((r1 >> j) & 1) +
                                   ((r2 >> (j + 1)) & 1) +
                                   ((r3 >> (j + 2)) & 1);
                        if (msum == 0) ok = false;

                        /* Anti‑diagonal sum, must not be three */
                        int asum = ((r1 >> (j + 2)) & 1) +
                                   ((r2 >> (j + 1)) & 1) +
                                   ((r3 >> j) & 1);
                        if (asum == 3) ok = false;
                    }
                    allowed[r1][r2][r3] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP tables for counts of sequences ending with (prev, curr) rows */
    seq_t dp_curr[16][16];
    seq_t dp_next[16][16];

    /* Initialise for length 2 (any pair of rows is allowed) */
    for (int rprev = 0; rprev < 16; ++rprev)
        for (int rcurr = 0; rcurr < 16; ++rcurr)
            dp_curr[rprev][rcurr] = (seq_t)1;

    /* Extend to length m (m ≥ 3) */
    for (idx_t i = 3; i <= m; ++i) {
        /* Zero dp_next */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                dp_next[a][b] = 0;

        for (int rprev = 0; rprev < 16; ++rprev) {
            for (int rcurr = 0; rcurr < 16; ++rcurr) {
                seq_t val = dp_curr[rprev][rcurr];
                if (val == 0) continue;
                for (int rnext = 0; rnext < 16; ++rnext) {
                    if (allowed[rprev][rcurr][rnext]) {
                        dp_next[rcurr][rnext] += val;
                    }
                }
            }
        }
        /* Move dp_next → dp_curr for next iteration */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                dp_curr[a][b] = dp_next[a][b];
    }

    /* Sum all counts for sequences of length m */
    seq_t total = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            total += dp_curr[a][b];

    return total;
}
