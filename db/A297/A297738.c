/*
 * A297738  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   col_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/col_valid.h"

/* Core: Count 6×n binary arrays where each 1 has exactly 2 or 4
 * adjacent 1s in the 6–neighbour (hexagonal) directions:
 * left, right, up-left, up-right, down-left, down-right. 
 * The function returns the exact count in the type seq_t. */

#define STATE_COUNT 4096               /* 2^12: two 6‑bit columns */
static int   next_list[STATE_COUNT][64];
static char  next_count[STATE_COUNT];
static bool  lists_built = false;

/* Pre‑compute, for each pair (prevprev,prev), the list of valid next
 * columns.  State is encoded as (prevprev << 6) | prev. */
static void build_next_lists(void)
{
    if (lists_built) return;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            int idx = (a << 6) | b;
            char cnt = 0;
            for (int c = 0; c < 64; ++c) {
                if (col_valid((uint8_t)a, (uint8_t)b, (uint8_t)c)) {
                    next_list[idx][cnt++] = c;
                }
            }
            next_count[idx] = cnt;
        }
    }
    lists_built = true;
}

seq_t a(idx_t n)
{
    build_next_lists();

    /* dp[s] = number of ways for processed columns ending in state
     *        (prevprev, prev).  After reading the first column c0 we
     *        store state (0, c0).  After each iteration we validate
     *        the middle column of each triple. */
    static seq_t dp[STATE_COUNT];
    static seq_t newdp[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    for (int c0 = 0; c0 < 64; ++c0) dp[(0 << 6) | c0] = 1;

    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATE_COUNT; ++i) newdp[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;

            int a = s >> 6;   /* left column of the pair */
            int b = s & 63;   /* middle column of the pair */
            char cnt = next_count[s];
            for (int k = 0; k < cnt; ++k) {
                int c = next_list[s][k];
                int new_s = (b << 6) | c;   /* new state: (prev, curr) */
                newdp[new_s] += ways;
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = newdp[i];
    }

    /* Final validation of the last column against a trailing zero column. */
    seq_t ans = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        seq_t ways = dp[s];
        if (ways == 0) continue;
        int prev = s >> 6;
        int curr = s & 63;
        if (col_valid((uint8_t)prev, (uint8_t)curr, 0)) {
            ans += ways;
        }
    }
    return ans;
}
