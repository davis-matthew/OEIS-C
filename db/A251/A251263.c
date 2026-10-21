/*
 * A251263  Number of (n+1)X(3+1) 0..1 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over binary rows of width 4; adjacency
   determined by the 2×2 subblock constraint x11‑x00 ≥ x10‑x01. */

#include <stdlib.h>
#include <stdbool.h>

static bool _adj[16][16];
static bool _adj_init = false;

/* Build the adjacency matrix once. */
static void _build_adj(void)
{
    if (_adj_init) return;
    for (unsigned int r1 = 0; r1 < 16; ++r1) {
        for (unsigned int r2 = 0; r2 < 16; ++r2) {
            bool ok = true;
            for (unsigned int j = 0; j < 3; ++j) {
                int a = (r1 >> j) & 1;
                int b = (r1 >> (j+1)) & 1;
                int c = (r2 >> j) & 1;
                int d = (r2 >> (j+1)) & 1;
                if (d - a < c - b) { ok = false; break; }
            }
            _adj[r1][r2] = ok;
        }
    }
    _adj_init = true;
}

seq_t a(idx_t n)
{
    /* n >= 1, height = n+1 rows */
    if (n == 0) return 0;
    _build_adj();

    seq_t dp[16], ndp[16];
    for (unsigned int i = 0; i < 16; ++i) dp[i] = 1;   /* first row arbitrary */

    for (idx_t step = 1; step <= n; ++step) {       /* add new row step times */
        for (unsigned int i = 0; i < 16; ++i) ndp[i] = 0;
        for (unsigned int prev = 0; prev < 16; ++prev) {
            if (dp[prev] == 0) continue;
            for (unsigned int nxt = 0; nxt < 16; ++nxt) {
                if (_adj[prev][nxt]) ndp[nxt] += dp[prev];
            }
        }
        for (unsigned int i = 0; i < 16; ++i) dp[i] = ndp[i];
    }

    seq_t res = 0;
    for (unsigned int i = 0; i < 16; ++i) res += dp[i];
    return res;
}
