/*
 * A254458  Number of tilings of a 6 X n rectangle using polyominoes of shape
 *          I.
 * https://oeis.org/A254458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   dfs_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the number of tilings of a 6×n rectangle
    with polyominoes of shape I (rectangles of width 1, height k).
    The algorithm is a transfer‑matrix dynamic programming:
    each column is represented by a 6‑bit mask that records which cells
    are occupied by a horizontal bar that started in a previous column.
    The transition from an incoming mask s to an outgoing mask t
    enumerates all ways to fill the free cells in the current column
    using vertical bars and horizontal bars that may start here. */

#include <stdbool.h>
#include "../utils/dfs_transition.h"

/* Main function */
seq_t a(idx_t n)
{
    /* 64 states (6 bits), pre‑computed once */
    static bool trans_built = false;
    static seq_t trans[64][64];    /* trans[s][t] = number of ways from s to t */

    if (!trans_built) {
        int cnt[64];
        for (int s = 0; s < 64; ++s) {
            for (int i = 0; i < 64; ++i)
                cnt[i] = 0;
            dfs_transition(0, s, 0, cnt);
            for (int t = 0; t < 64; ++t)
                trans[s][t] = (seq_t)cnt[t];
        }
        trans_built = true;
    }

    if (n == 0)
        return (seq_t)1;              /* empty rectangle */

    /* DP arrays: current and next column */
    static seq_t dp_cur[64];
    static seq_t dp_next[64];
    for (int i = 0; i < 64; ++i)
        dp_cur[i] = 0;
    dp_cur[0] = (seq_t)1;             /* start with no pending bars */

    for (idx_t col = 0; col < n; ++col) {
        for (int i = 0; i < 64; ++i)
            dp_next[i] = 0;

        for (int s = 0; s < 64; ++s) if (dp_cur[s] != (seq_t)0) {
            for (int t = 0; t < 64; ++t) if (trans[s][t] != (seq_t)0) {
                dp_next[t] += dp_cur[s] * trans[s][t];
            }
        }

        /* prepare for next column */
        for (int i = 0; i < 64; ++i)
            dp_cur[i] = dp_next[i];
    }

    return dp_cur[0];                 /* no unfinished bars after last column */
}
