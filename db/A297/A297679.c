/*
 * A297679  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 4 neighboring 1s.
 * https://oeis.org/A297679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   count_valid_neighbors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297679
    Number of n×5 binary arrays with every 1 horizontally, diagonally or antidiagonally
    adjacent to 0, 1 or 4 neighboring 1s.
    (The definition uses only the 6 neighbors: left, right, up‑left, up‑right,
     down‑left, down‑right – vertical adjacency is **not** considered.)  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_valid_neighbors.h"

static inline int valid_row(int prev, int curr, int nxt)
{
    for (int col = 0; col < 5; ++col) {
        if (curr & (1u << col)) {
            int c = count_valid_neighbors(prev, curr, nxt, col);
            if (c != 0 && c != 1 && c != 4) return 0;
        }
    }
    return 1;
}

/* 32×32 table of admissible next‑row masks for each (prev, curr) pair.  */
static uint32_t next_mask[32][32];
static bool masks_prepared = false;

static void precompute_masks(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            uint32_t mask = 0;
            for (int nxt = 0; nxt < 32; ++nxt)
                if (valid_row(prev, curr, nxt))
                    mask |= (1u << nxt);
            next_mask[prev][curr] = mask;
        }
    }
}

static inline void zero_matrix(seq_t m[32][32])
{
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            m[i][j] = 0;
}

static inline void copy_matrix(seq_t dst[32][32], seq_t src[32][32])
{
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dst[i][j] = src[i][j];
}

/*  Compute a(n) for n ≥ 1.  */
seq_t a(idx_t n)
{
    if (!masks_prepared) {
        precompute_masks();
        masks_prepared = true;
    }

    seq_t dp[32][32];
    seq_t newdp[32][32];
    zero_matrix(dp);
    zero_matrix(newdp);

    /* Base state: empty previous row (all zeros), current row arbitrary. */
    for (int cur = 0; cur < 32; ++cur)
        dp[0][cur] = 1;

    /* Process subsequent rows.  For each step we add a new row. */
    for (idx_t r = 2; r <= n; ++r) {
        zero_matrix(newdp);
        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t val = dp[prev][cur];
                if (val == 0) continue;
                uint32_t mask = next_mask[prev][cur];
                while (mask) {
                    uint32_t bit = mask & -mask;     /* lowest set bit */
                    int nxt = __builtin_ctz(bit);
                    newdp[cur][nxt] += val;
                    mask -= bit;
                }
            }
        }
        copy_matrix(dp, newdp);
    }

    /* Account for the last row (no next row).  */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t val = dp[prev][cur];
            if (val == 0) continue;
            if (valid_row(prev, cur, 0))
                result += val;
        }
    }
    return result;
}
