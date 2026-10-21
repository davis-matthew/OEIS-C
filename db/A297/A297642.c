/*
 * A297642  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A297642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays in which every 1 has 0, 2 or
 *            3 neighboring 1s horizontally or diagonally. Computed by a
 *            two‑step transfer‑matrix dynamic programming
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Transition tables (state = (prev_col << 6) | cur_col). */
static uint8_t transCnt[4096];
static uint8_t trans[4096][64];
static bool trans_built = false;

/* Check whether column 'cur' satisfies the adjacency rule with
   left neighbor 'left' and right neighbor 'right'. */
static inline bool
valid(int left, int cur, int right)
{
    for (int r = 0; r < 6; ++r) {
        if (!((cur >> r) & 1))
            continue;               /* cell is 0 → no restriction */

        int nb = 0;
        nb += (left >> r) & 1;          /* left, same row */
        nb += (right >> r) & 1;         /* right, same row */

        if (r > 0) {                    /* up-left and up-right */
            nb += (left >> (r - 1)) & 1;
            nb += (right >> (r - 1)) & 1;
        }
        if (r < 5) {                    /* down-left and down-right */
            nb += (left >> (r + 1)) & 1;
            nb += (right >> (r + 1)) & 1;
        }

        if (nb == 1 || nb >= 4)         /* only 0,2,3 allowed */
            return false;
    }
    return true;
}

/* Build transition tables once. */
static void
build_transitions(void)
{
    if (trans_built)
        return;
    for (int left = 0; left < 64; ++left) {
        for (int cur = 0; cur < 64; ++cur) {
            int idx = (left << 6) | cur;
            uint8_t count = 0;
            for (int right = 0; right < 64; ++right) {
                if (valid(left, cur, right))
                    trans[idx][count++] = (uint8_t)right;
            }
            transCnt[idx] = count;
        }
    }
    trans_built = true;
}

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    build_transitions();

    static seq_t dp[4096];
    static seq_t newdp[4096];

    /* Initialize DP with first column (left neighbor is dummy zero). */
    for (int i = 0; i < 4096; ++i)
        dp[i] = (seq_t)0;
    for (int col = 0; col < 64; ++col)
        dp[col] = (seq_t)1;             /* (0 << 6) | col == col */

    /* Process subsequent columns. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 4096; ++i)
            newdp[i] = (seq_t)0;

        for (int idx = 0; idx < 4096; ++idx) {
            seq_t val = dp[idx];
            if (val == (seq_t)0)
                continue;
            int prev = idx >> 6;
            int cur  = idx & 63;
            uint8_t tcnt = transCnt[idx];
            const uint8_t *ptr = trans[idx];
            for (int t = 0; t < tcnt; ++t) {
                int nxt = ptr[t];
                int newidx = (cur << 6) | nxt;
                newdp[newidx] += val;
            }
        }
        /* Move to next step. */
        for (int i = 0; i < 4096; ++i)
            dp[i] = newdp[i];
    }

    /* Final boundary check: right neighbor is dummy zero. */
    seq_t result = (seq_t)0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t val = dp[idx];
        if (val == (seq_t)0)
            continue;
        int prev = idx >> 6;
        int cur  = idx & 63;
        if (valid(prev, cur, 0))
            result += val;
    }
    return result;
}
