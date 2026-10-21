/*
 * A297805  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays in which every 1 has 2, 3 or
 *            5 neighbors among horizontally and diagonally adjacent cells.
 *            Computed by a 5‑column transfer‑matrix
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   neighbors_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/neighbors_ok.h"

static uint32_t nextmask[1024];
static int nextmask_initialized = 0;

static void init_nextmask(void) {
    if (nextmask_initialized) return;
    for (uint32_t a = 0; a < 32; ++a) {
        for (uint32_t b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (uint32_t c = 0; c < 32; ++c) {
                if (neighbors_ok(a, b, c))
                    mask |= (1u << c);
            }
            nextmask[(a << 5) | b] = mask;
        }
    }
    nextmask_initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* zero index is undefined; return 0 */
    init_nextmask();

    /* DP arrays for current and next column counts */
    seq_t dp[1024]      = {0};
    seq_t dp_next[1024] = {0};

    /* Initial column (first real column).  Left side is all zeros (a=0). */
    for (uint32_t c = 0; c < 32; ++c) {
        /* state (a=0, b=c) */
        dp[(0 << 5) | c] = 1;
    }

    /* Process remaining columns (col index 1 .. n-1) */
    for (idx_t idx = 1; idx < n; ++idx) {
        /* reset dp_next */
        for (int i = 0; i < 1024; ++i) dp_next[i] = 0;

        for (int id = 0; id < 1024; ++id) {
            seq_t cur = dp[id];
            if (cur == 0) continue;
            uint32_t b = id & 31U;
            uint32_t mask = nextmask[id];
            while (mask) {
                uint32_t bit = __builtin_ctz(mask);
                mask &= mask - 1;
                int newid = (b << 5) | bit;   /* (b, c) -> new state */
                dp_next[newid] = dp_next[newid] + cur;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 1024; ++i) dp[i] = dp_next[i];
    }

    /* Final dummy column of zeros to validate the last real column. */
    seq_t result = 0;
    for (int id = 0; id < 1024; ++id) {
        if (dp[id] == 0) continue;
        /* check if transition to c = 0 is allowed */
        if (nextmask[id] & 1u) {          /* bit 0 set means c=0 allowed */
            result = result + dp[id];
        }
    }
    return result;
}
