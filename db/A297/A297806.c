/*
 * A297806  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   valid_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A297806: the number of 6×n binary arrays such that
   every 1 has exactly 2, 3, or 5 neighbours that are 1 among
   its horizontal, nor‑right or anti‑diagonal neighbours.
   The algorithm uses a column‑wise transfer‑matrix dynamic
   programme.  For each pair of adjacent columns we keep a state
   consisting of the configuration of the left and the middle
   columns.  Transition tables are built once for all possible
   states and then reused for every n.  No heap allocations are
   performed. */

#include <stdint.h>
#include "../utils/valid_column.h"

/* Transition lists for all 4096 states (prev,curr).
   Each list contains the next column values that make
   the middle column valid.  The lists are built the first
   time a is called. */
static uint8_t trans_next[4096][64];
static uint8_t trans_cnt[4096];
static int trans_done = 0;

static void build_transitions(void)
{
    for (int state = 0; state < 4096; ++state) {
        uint8_t prev = state >> 6;
        uint8_t curr = state & 0x3F;
        uint8_t cnt = 0;
        for (uint8_t next = 0; next < 64; ++next) {
            if (valid_column(prev, curr, next)) {
                trans_next[state][cnt++] = next;
            }
        }
        trans_cnt[state] = cnt;
    }
    trans_done = 1;
}

seq_t a(idx_t n)
{
    /* One dimension is 2^12 = 4096 states. */
    static seq_t dp[4096];
    static seq_t ndp[4096];

    /* First call: build transition tables. */
    if (!trans_done) build_transitions();

    /* Initialise dp: left column is 0, middle column arbitrary. */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;
    for (uint8_t curr = 0; curr < 64; ++curr) dp[curr] = 1; /* prev = 0 */

    /* Process columns 2 … n. */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear ndp */
        for (int i = 0; i < 4096; ++i) ndp[i] = 0;
        for (int state = 0; state < 4096; ++state) {
            seq_t val = dp[state];
            if (val == 0) continue;
            uint8_t curr = state & 0x3F;
            /* iterate over allowed next columns */
            uint8_t* next_list = trans_next[state];
            uint8_t c = trans_cnt[state];
            for (uint8_t j = 0; j < c; ++j) {
                uint8_t next = next_list[j];
                int new_state = (curr << 6) | next;
                ndp[new_state] += val;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 4096; ++i) dp[i] = ndp[i];
    }

    /* Final column: right neighbour is 0. */
    seq_t ans = 0;
    for (int state = 0; state < 4096; ++state) {
        seq_t val = dp[state];
        if (val == 0) continue;
        uint8_t prev = state >> 6;
        uint8_t curr = state & 0x3F;
        if (valid_column(prev, curr, 0))
            ans += val;
    }
    return ans;
}
