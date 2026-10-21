/*
 * A250969  Number of (n+1)X(4+1) 0..1 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A250969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows for 0/1 (n+1)×5 matrices such that for every 2×2
 *            subblock with entries a,b,c,d (top‑left, top‑right, bottom‑left,
 *            bottom‑right) we have a + d >= |b – c|. Only consecutive rows
 *            interact, so we pre‑compute an adjacency matrix of allowed row
 *            transitions and perform dynamic programming
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    enum {W = 5};                 /* number of columns */
    enum {R = 1 << W};            /* number of possible 5‑bit rows (32) */

    /* Precompute allowed transitions between row patterns */
    static bool trans[R][R];
    static bool init = false;
    if (!init) {
        for (int a = 0; a < R; ++a) {
            for (int b = 0; b < R; ++b) {
                bool ok = true;
                for (int col = 0; col < W - 1; ++col) {
                    int tl = (a >> col) & 1;     /* top‑left     */
                    int tr = (a >> (col + 1)) & 1; /* top‑right   */
                    int bl = (b >> col) & 1;     /* bottom‑left  */
                    int br = (b >> (col + 1)) & 1; /* bottom‑right */
                    if (tr != bl && tl == 0 && br == 0) {
                        ok = false;
                        break;
                    }
                }
                trans[a][b] = ok;
            }
        }
        init = true;
    }

    /* DP arrays: dp[r] = number of ways to reach row pattern r */
    seq_t dp[R], ndp[R];
    for (int i = 0; i < R; ++i) dp[i] = 1;  /* first row can be any pattern */

    for (idx_t step = 0; step < n; ++step) { /* n transitions => (n+1) rows */
        for (int i = 0; i < R; ++i) ndp[i] = 0;
        for (int prev = 0; prev < R; ++prev) {
            if (dp[prev] == 0) continue;
            seq_t ways = dp[prev];
            for (int next = 0; next < R; ++next) {
                if (trans[prev][next]) ndp[next] += ways;
            }
        }
        for (int i = 0; i < R; ++i) dp[i] = ndp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < R; ++i) total += dp[i];
    return total;
}
