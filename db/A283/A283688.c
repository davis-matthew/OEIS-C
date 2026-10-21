/*
 * A283688  Number of nX5 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  DP over rows: for each row keep the pattern of the last two rows.
    First row must be valid with no previous row but with the second.
    Subsequent rows are validated when they become the middle row.
    The last row is validated with no following row. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>    /* memcpy, memset */

/* check if a 1 in "row" has at most two neighbours among
   horizontal (left/right) and the four diagonals from the
   previous and next rows.  The arguments "prev" and "next"
   provide the row above and below, or 0 if there is none. */
static inline bool
row_ok(uint32_t row, uint32_t prev, uint32_t next)
{
    for (int j = 0; j < 5; ++j) {
        if ((row >> j) & 1U) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0 && (row >> (j - 1)) & 1U) ++cnt;
            if (j < 4 && (row >> (j + 1)) & 1U) ++cnt;
            /* diagonal neighbours from the previous row */
            if (j > 0 && (prev >> (j - 1)) & 1U) ++cnt;
            if (j < 4 && (prev >> (j + 1)) & 1U) ++cnt;
            /* diagonal neighbours from the next row */
            if (j > 0 && (next >> (j - 1)) & 1U) ++cnt;
            if (j < 4 && (next >> (j + 1)) & 1U) ++cnt;
            if (cnt > 2) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) = 32 (all patterns are valid) */
    if (n == 1U) {
        return (seq_t)32U;
    }

    /* pre‑compute valid transitions: valid[p*32+c][nx] = row_ok(c,p,nx) */
    static bool valid[1024][32];
    static bool ready = false;
    if (!ready) {
        for (uint32_t p = 0; p < 32; ++p) {
            for (uint32_t c = 0; c < 32; ++c) {
                uint32_t idx = p * 32U + c;
                for (uint32_t nx = 0; nx < 32; ++nx) {
                    valid[idx][nx] = row_ok(c, p, nx);
                }
            }
        }
        ready = true;
    }

    /* n == 2 : brute force (first and last rows are adjacent only) */
    if (n == 2U) {
        seq_t tot = 0;
        for (uint32_t r1 = 0; r1 < 32; ++r1) {
            for (uint32_t r2 = 0; r2 < 32; ++r2) {
                if (row_ok(r1, 0U, r2) && row_ok(r2, r1, 0U))
                    tot += (seq_t)1U;
            }
        }
        return tot;
    }

    /* DP for n >= 3 */
    static seq_t dp[1024];
    static seq_t nxt[1024];
    /* initialise dp with pairs (row1,row2) that make row1 valid */
    for (int i = 0; i < 1024; ++i) dp[i] = 0;
    for (uint32_t r1 = 0; r1 < 32; ++r1) {
        for (uint32_t r2 = 0; r2 < 32; ++r2) {
            if (row_ok(r1, 0U, r2))
                dp[r1 * 32U + r2] = (seq_t)1U;
        }
    }

    for (idx_t step = 3; step <= n; ++step) {
        memset(nxt, 0, sizeof(nxt));
        for (uint32_t idx = 0; idx < 1024; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            uint32_t prev = idx / 32U;
            uint32_t curr = idx % 32U;
            for (uint32_t nx = 0; nx < 32; ++nx) {
                if (valid[idx][nx]) {
                    nxt[curr * 32U + nx] += cur;
                }
            }
        }
        memcpy(dp, nxt, sizeof(dp));
    }

    /* final validation of the last row */
    seq_t ans = 0;
    for (uint32_t idx = 0; idx < 1024; ++idx) {
        seq_t cur = dp[idx];
        if (cur == 0) continue;
        uint32_t prev = idx / 32U;
        uint32_t curr = idx % 32U;
        if (row_ok(curr, prev, 0U))
            ans += cur;
    }
    return ans;
}
