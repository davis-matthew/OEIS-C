/*
 * A251067  Number of (3+1)X(n+1) 0..2 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The number of 4×(n+1) arrays with entries 0,1,2 in which for
 *            every 2×2 block the inequality x11 + x01 ≥ x10 + x00 holds. This
 *            is computed by a dynamic programme over the 81 possible column
 *            vectors (four digits in base‑3). Let A be the 81×81 adjacency
 *            matrix where A[i][j] = 1 iff the column j may...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 27, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The parameter types are defined externally by the build system. */
extern seq_t a(idx_t n);

static bool adj[81][81];
static bool adj_inited = false;

/* Initialize adjacency matrix once. */
static void init_adj(void)
{
    for (int i = 0; i < 81; ++i) {
        int ai[4]; int val = i;
        for (int r = 0; r < 4; ++r) { ai[r] = val % 3; val /= 3; }
        for (int j = 0; j < 81; ++j) {
            int bj[4]; int val2 = j;
            for (int r = 0; r < 4; ++r) { bj[r] = val2 % 3; val2 /= 3; }
            bool ok = true;
            for (int r = 0; r < 3 && ok; ++r) {
                if (bj[r+1] + bj[r] < ai[r+1] + ai[r])
                    ok = false;
            }
            adj[i][j] = ok;
        }
    }
    adj_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!adj_inited) init_adj();

    seq_t dp[81];
    seq_t next[81];

    /* first column: any of the 81 states. */
    for (int i = 0; i < 81; ++i) dp[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        for (int j = 0; j < 81; ++j) next[j] = (seq_t)0;
        for (int i = 0; i < 81; ++i) {
            if (dp[i] == (seq_t)0) continue;
            for (int j = 0; j < 81; ++j) {
                if (adj[i][j]) {
                    next[j] = next[j] + dp[i];
                }
            }
        }
        for (int j = 0; j < 81; ++j) dp[j] = next[j];
    }

    seq_t res = (seq_t)0;
    for (int i = 0; i < 81; ++i) res = res + dp[i];
    return res;
}
