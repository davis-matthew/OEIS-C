/*
 * A296035  Number of nX4 0..1 arrays with each 1 adjacent to 2 or 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A296035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A296035: Number of n×4 0..1 arrays with each 1 adjacent
   to 2 or 4 king‑move neighboring 1s.
   Computed by a transfer‑matrix dynamic programming over column masks.
   The DP state is a pair of 4‑bit masks (previous column, current column).
   Transition constraint checks that all 1s in the current column have
   exactly 2 or 4 1‑neighbours once the next column is added.
   For the final column we also verify this property with no right neighbor.
*/

#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* Pre‑computed adjacency and final‑check tables. */
static bool    final_ok[256];
static uint8_t next_cnt[256];
static uint8_t next_masks[256][16];
static bool    initialized = false;

static void
init_tables(void)
{
    if (initialized)
        return;

    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            int state = (prev << 4) | cur;

            /* Final validity for the last column (no right neighbours). */
            bool ok = true;
            for (int r = 0; r < 4 && ok; ++r) {
                int bit = (cur >> r) & 1;
                if (!bit)
                    continue;
                int left = 0;
                if (r > 0) left += (prev >> (r - 1)) & 1;
                left += (prev >> r) & 1;
                if (r < 3) left += (prev >> (r + 1)) & 1;
                if (r > 0) left += (cur >> (r - 1)) & 1;
                if (r < 3) left += (cur >> (r + 1)) & 1;
                int total = left;                  /* no right part */
                if (!(total == 2 || total == 4))
                    ok = false;
            }
            final_ok[state] = ok;

            /* Transition table: which next masks are allowed. */
            uint8_t cnt = 0;
            for (int next = 0; next < 16; ++next) {
                bool valid = true;
                for (int r = 0; r < 4 && valid; ++r) {
                    int bit = (cur >> r) & 1;
                    if (!bit)
                        continue;
                    int left = 0;
                    if (r > 0) left += (prev >> (r - 1)) & 1;
                    left += (prev >> r) & 1;
                    if (r < 3) left += (prev >> (r + 1)) & 1;
                    if (r > 0) left += (cur >> (r - 1)) & 1;
                    if (r < 3) left += (cur >> (r + 1)) & 1;
                    int right = 0;
                    if (r > 0) right += (next >> (r - 1)) & 1;
                    right += (next >> r) & 1;
                    if (r < 3) right += (next >> (r + 1)) & 1;
                    int total = left + right;
                    if (!(total == 2 || total == 4))
                        valid = false;
                }
                if (valid) {
                    next_masks[state][cnt++] = (uint8_t)next;
                }
            }
            next_cnt[state] = cnt;
        }
    }
    initialized = true;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    init_tables();

    /* Arrays holding current and next DP layer. */
    seq_t dp_curr[256];
    seq_t dp_next[256];

    /* Initialise for the first column: previous column mask = 0. */
    for (int i = 0; i < 256; ++i) {
        dp_curr[i] = 0;
        dp_next[i] = 0;
    }
    for (int cur = 0; cur < 16; ++cur) {
        int state = (0 << 4) | cur;   /* prev = 0 */
        dp_curr[state] = 1;
    }

    /* Process columns 2 … n. */
    for (idx_t col = 2; col <= n; ++col) {
        /* Clear dp_next */
        for (int i = 0; i < 256; ++i)
            dp_next[i] = 0;

        for (int state = 0; state < 256; ++state) {
            seq_t ways = dp_curr[state];
            if (ways == 0)
                continue;
            uint8_t cnt = next_cnt[state];
            for (uint8_t k = 0; k < cnt; ++k) {
                int next = next_masks[state][k];
                int cur = state & 0xF;               /* current column mask */
                int nxt_state = (cur << 4) | next;   /* prev = cur, cur = next */
                dp_next[nxt_state] += ways;
            }
        }
        /* Move next layer to current. */
        for (int i = 0; i < 256; ++i)
            dp_curr[i] = dp_next[i];
    }

    /* Accumulate final answers where the last column satisfies constraints. */
    seq_t ans = 0;
    for (int state = 0; state < 256; ++state) {
        if (final_ok[state])
            ans += dp_curr[state];
    }
    return ans;
}
