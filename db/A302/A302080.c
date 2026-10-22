/*
 * A302080  Number of nX7 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 arrays of 0/1 with upper‑left cell 0
 *            satisfying that each cell has 0,1,3 or 4 equal horizontally or
 *            antidiagonally adjacent neighbours. Computed by a linear–state
 *            DP
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Transition bitmask: for each pair (prev_row, curr_row) we store a 128‑bit
   mask of all next_row values that make curr_row valid.  The mask is
   represented by two 64‑bit words to support all 128 next_row patterns. */
static unsigned long long next_bits[128][128][2];
static bool precomputed = false;

/* Helper: test whether a row is valid given its neighbours.
   has_prev / has_next indicate whether the corresponding rows exist. */
static bool row_valid(uint8_t prev, bool has_prev,
                      uint8_t curr, bool has_next, uint8_t next)
{
    for (int j = 0; j < 7; ++j) {
        int val = (curr >> j) & 1;
        int cnt = 0;
        /* left neighbour */
        if (j > 0)          cnt += ((curr >> (j-1)) & 1) == val;
        /* right neighbour */
        if (j < 6)          cnt += ((curr >> (j+1)) & 1) == val;
        /* upper‑left neighbour */
        if (has_prev && j < 6) cnt += ((prev >> (j+1)) & 1) == val;
        /* lower‑right neighbour */
        if (has_next && j > 0) cnt += ((next >> (j-1)) & 1) == val;
        if (!(cnt == 0 || cnt == 1 || cnt == 3 || cnt == 4))
            return false;
    }
    return true;
}

/* Pre‑compute all transition masks. */
static void precompute(void)
{
    if (precomputed) return;
    for (int prev = 0; prev < 128; ++prev) {
        for (int curr = 0; curr < 128; ++curr) {
            unsigned long long mask_lo = 0, mask_hi = 0;
            for (int next = 0; next < 128; ++next) {
                if (row_valid((uint8_t)prev, true,
                              (uint8_t)curr, true,
                              (uint8_t)next)) {
                    if (next < 64) mask_lo |= 1ULL << next;
                    else mask_hi |= 1ULL << (next - 64);
                }
            }
            next_bits[prev][curr][0] = mask_lo;
            next_bits[prev][curr][1] = mask_hi;
        }
    }
    precomputed = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; values for n>=1 only */

    precompute();

    /* DP arrays: dp[p][c] counts number of ways for the last two rows
       being prev==p, curr==c. */
    static seq_t dp_prev[128][128];
    static seq_t dp_cur [128][128];
    memset(dp_prev, 0, sizeof(dp_prev));
    memset(dp_cur , 0, sizeof(dp_cur ));

    /* Handle n == 1 separately */
    if (n == 1) {
        seq_t ans = 0;
        for (int m = 0; m < 128; ++m) {
            if ((m & 1) == 0) {   /* upper‑left cell (bit 0) is 0 */
                if (row_valid(0, false, (uint8_t)m, false, 0))
                    ans += 1;
            }
        }
        return ans;
    }

    /* n >= 2: initialise with row 0 (prev) and row 1 (curr) */
    for (int m0 = 0; m0 < 128; ++m0) {
        if ((m0 & 1) == 0) {   /* upper‑left zero restriction */
            for (int m1 = 0; m1 < 128; ++m1) {
                if (row_valid(0, false, (uint8_t)m0, true, (uint8_t)m1))
                    dp_prev[m0][m1] += 1;
            }
        }
    }

    /* Process rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row) {
        /* clear current DP */
        memset(dp_cur, 0, sizeof(dp_cur));

        for (int prev = 0; prev < 128; ++prev) {
            for (int cur  = 0; cur  < 128; ++cur) {
                seq_t cnt = dp_prev[prev][cur];
                if (!cnt) continue;
                /* iterate over allowed next rows */
                unsigned long long mask_lo = next_bits[prev][cur][0];
                unsigned long long mask_hi = next_bits[prev][cur][1];
                /* low part */
                while (mask_lo) {
                    int next = __builtin_ctzll(mask_lo);
                    mask_lo &= mask_lo - 1;
                    dp_cur[cur][next] += cnt;
                }
                /* high part */
                while (mask_hi) {
                    int next = 64 + __builtin_ctzll(mask_hi);
                    mask_hi &= mask_hi - 1;
                    dp_cur[cur][next] += cnt;
                }
            }
        }
        /* swap dp_prev and dp_cur */
        memcpy(dp_prev, dp_cur, sizeof(dp_prev));
    }

    /* Final row (row n-1) needs bottom boundary check */
    seq_t ans = 0;
    for (int prev = 0; prev < 128; ++prev) {
        for (int cur  = 0; cur  < 128; ++cur) {
            seq_t cnt = dp_prev[prev][cur];
            if (!cnt) continue;
            if (row_valid((uint8_t)prev, true,
                          (uint8_t)cur , false, 0))
                ans += cnt;
        }
    }
    return ans;
}
