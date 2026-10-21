/*
 * A295547  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 3 or 4 1s.
 * https://oeis.org/A295547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A295547: Number of n×4 0..1 arrays with each 1 horizontally or vertically adjacent to 1, 3 or 4 1s.
   Computed by a transfer‑matrix DP over rows.  The state is a pair (prev_row, curr_row);
   when a new row is added we test that all 1s in the middle row have an allowed number
   of 1‑neighbours (1, 3 or 4) using the previous, current, and new rows.  The DP
   needs only 16×16 = 256 states and 16 outgoing transitions for each state.
*/

#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: test whether the middle row 'mid' is valid when surrounded
   by 'up' and 'down'.  All indices are 4‑bit strings (0..15).     */
static inline bool is_valid(uint8_t up, uint8_t mid, uint8_t down)
{
    for (int col = 0; col < 4; ++col) {
        if (!(mid & (1u << col))) continue;           /* only cells containing 1 */
        unsigned cnt = 0;
        /* horizontal neighbours */
        if (col > 0 && (mid & (1u << (col - 1)))) ++cnt;
        if (col < 3 && (mid & (1u << (col + 1)))) ++cnt;
        /* vertical neighbours */
        if (up   & (1u << col)) ++cnt;
        if (down & (1u << col)) ++cnt;
        if (!(cnt == 1 || cnt == 3 || cnt == 4)) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute the allowed transitions.  allowed_next[u][m] is a 16‑bit
   mask of down rows that make the middle row 'm' valid with up row 'u'.
   This table is computed once and reused for all calls. */
static unsigned short allowed_next[16][16];
static bool allowed_table_inited = false;

static void init_allowed_table(void)
{
    if (allowed_table_inited) return;
    for (int u = 0; u < 16; ++u) {
        for (int m = 0; m < 16; ++m) {
            unsigned short mask = 0;
            for (int d = 0; d < 16; ++d) {
                if (is_valid((uint8_t)u, (uint8_t)m, (uint8_t)d))
                    mask |= (1u << d);
            }
            allowed_next[u][m] = mask;
        }
    }
    allowed_table_inited = true;
}

/* ------------------------------------------------------------------ */
/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    /* a(0) is undefined by the OEIS offset; return 0 for safety. */
    if (n == 0) return (seq_t)0;
    init_allowed_table();

    /* DP arrays: dp[prev*16 + curr] = number of ways. */
    static seq_t dp[256];
    static seq_t dp_next[256];

    /* initialise for the first row: previous row is 0 (dummy). */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;
    for (int curr = 0; curr < 16; ++curr) {
        dp[curr] = (seq_t)1;   /* prev = 0, curr = pattern */
    }

    /* iterate over rows 2..n (first row already initialised). */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev * 16 + curr];
                if (val == (seq_t)0) continue;
                unsigned short mask = allowed_next[prev][curr];
                unsigned short m = mask;
                while (m) {
                    unsigned down = __builtin_ctz(m); /* index of least set bit */
                    m &= m - 1;
                    dp_next[curr * 16 + down] += val;
                }
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 256; ++i) dp[i] = dp_next[i];
    }

    /* After the last real row, add a dummy bottom row (0) and sum all valid endings. */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t val = dp[prev * 16 + curr];
            if (val == (seq_t)0) continue;
            if (allowed_next[prev][curr] & 1u)           /* bit for down = 0 set */
                result += val;
        }
    }
    return result;
}
