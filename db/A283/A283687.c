/*
 * A283687  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 binary arrays with the property that each 1 has at
 *            most two of its * horizontal, diagonal and antidiagonal
 *            neighbors equal to 1. The sequence is generated * via dynamic
 *            programming on row patterns (transfer‑matrix)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return (seq_t)16;   /* all 2^4 patterns are valid */

    /* Precompute all valid triples (prev2, prev1, next) */
    static bool valid[16][16][16];
    static bool ready = false;
    if (!ready) {
        ready = true;
        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                for (int c = 0; c < 16; ++c) {
                    bool ok = true;
                    for (int j = 0; j < 4 && ok; ++j) {
                        if ((b >> j) & 1) {
                            int cnt = 0;
                            /* horizontal */
                            if (j > 0 && ((b >> (j - 1)) & 1)) ++cnt;
                            if (j < 3 && ((b >> (j + 1)) & 1)) ++cnt;
                            /* diagonal up-left/up-right from row a */
                            if (j > 0 && ((a >> (j - 1)) & 1)) ++cnt;
                            if (j < 3 && ((a >> (j + 1)) & 1)) ++cnt;
                            /* antidiagonal down-left/down-right from row c */
                            if (j > 0 && ((c >> (j - 1)) & 1)) ++cnt;
                            if (j < 3 && ((c >> (j + 1)) & 1)) ++cnt;
                            if (cnt > 2) ok = false;
                        }
                    }
                    valid[a][b][c] = ok;
                }
            }
        }
    }

    /* DP arrays: dp_cur[a*16 + b] holds number of ways to reach state (a,b) */
    seq_t dp_cur[256] = {0};
    seq_t dp_next[256] = {0};

    /* Initialize with first row (prev2 = 0) */
    for (int b = 0; b < 16; ++b) dp_cur[b] = 1;   /* index = 0*16 + b */

    /* Add rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                seq_t ways = dp_cur[a * 16 + b];
                if (!ways) continue;
                for (int c = 0; c < 16; ++c) {
                    if (valid[a][b][c]) dp_next[b * 16 + c] += ways;
                }
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final check: last row with below row = 0 */
    seq_t result = 0;
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            if (valid[a][b][0]) result += dp_cur[a * 16 + b];
        }
    }
    return result;
}
