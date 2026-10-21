/*
 * A295094  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 3 1s.
 * https://oeis.org/A295094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A295094: Number of n×5 binary arrays with each 1
 * horizontally or vertically adjacent to either 1 or 3 1s.
 *
 * The algorithm uses a transfer‑matrix dynamic programming over
 * columns.  Each state is a pair (prevprev, prev) of 5‑bit
 * column patterns (previous two columns).  A transition to a
 * new column 'curr' is allowed iff every 1 in 'prev' has
 * odd number of 1 neighbours (1 or 3) after adding the left
 * neighbour 'prevprev' and the right neighbour 'curr'.
 *
 * The state space is 32×32 = 1024; the transition mask for each
 * state is pre–computed once.  The DP runs in O(n·S·A) time
 * (S = 1024, A ≈ 10) which is well below the limits even for
 * n = 10000.
 */

#include <stdint.h>
#include <stdbool.h>

static bool _a295094_init = false;
static uint32_t _a295094_masks[1024];
static uint8_t  _a295094_vert[32][5];

/* Pre‑compute the transition masks and vertical neighbour counts. */
static void _a295094_precompute(void)
{
    for (int b = 0; b < 32; ++b) {
        for (int r = 0; r < 5; ++r) {
            int cnt = 0;
            if (r > 0) cnt += (b >> (r - 1)) & 1;
            if (r < 4) cnt += (b >> (r + 1)) & 1;
            _a295094_vert[b][r] = (uint8_t)cnt;
        }
    }

    for (int pp = 0; pp < 32; ++pp) {
        for (int p = 0; p < 32; ++p) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    if (p & (1 << r)) {
                        int cnt = _a295094_vert[p][r];
                        if (pp & (1 << r)) cnt++;
                        if (c & (1 << r)) cnt++;
                        if (cnt != 1 && cnt != 3) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) mask |= (1u << c);
            }
            _a295094_masks[(pp << 5) | p] = mask;
        }
    }

    _a295094_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!_a295094_init) _a295094_precompute();

    /* DP arrays: dp[cur][state] */
    static seq_t dp_cur[1024];
    static seq_t dp_next[1024];

    /* initialize for first column: left column is all zeros */
    for (int i = 0; i < 1024; ++i) dp_cur[i] = 0;
    for (int p = 0; p < 32; ++p) {
        int idx = p; /* prevprev = 0 */
        dp_cur[idx] = 1;
    }

    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < 1024; ++i) dp_next[i] = 0;
        for (int state = 0; state < 1024; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0) continue;
            uint32_t mask = _a295094_masks[state];
            if (mask == 0) continue;
            int prev = state & 31;
            for (int bit = 0; bit < 32; ++bit) {
                if (mask & (1u << bit)) {
                    int new_state = (prev << 5) | bit;
                    dp_next[new_state] += val;
                }
            }
        }
        /* swap */
        for (int i = 0; i < 1024; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final verification: right neighbour is zero */
    seq_t result = 0;
    if (n == 1) {
        for (int p = 0; p < 32; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;
            bool ok = true;
            for (int r = 0; r < 5; ++r) {
                if (p & (1 << r)) {
                    int cnt = _a295094_vert[p][r];
                    if (!(cnt == 1 || cnt == 3)) { ok = false; break; }
                }
            }
            if (ok) result += val;
        }
    } else {
        for (int state = 0; state < 1024; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0) continue;
            int pp = state >> 5;
            int p  = state & 31;
            bool ok = true;
            for (int r = 0; r < 5; ++r) {
                if (p & (1 << r)) {
                    int cnt = _a295094_vert[p][r];
                    if (pp & (1 << r)) cnt++;
                    if (!(cnt == 1 || cnt == 3)) { ok = false; break; }
                }
            }
            if (ok) result += val;
        }
    }

    return result;
}
