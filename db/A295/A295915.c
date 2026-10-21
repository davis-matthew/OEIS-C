/*
 * A295915  Number of nX5 0..1 arrays with each 1 adjacent to 0 or 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A295915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays where every 1 has 0 or 3
 *            king‑move neighboring 1s. Computed by dynamic programming with
 *            state equal to the last two rows
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int WIDTH = 5;
static const int PAT   = 1 << WIDTH;  /* 32 possible row patterns */

/* Count 1‑neighbors of a cell (row=curr) column=j given its neighboring rows. */
static inline int count_neighbors(int prev, int curr, int next, int j) {
    int count = 0;
    if (j > 0 && (prev  & (1 << (j - 1)))) count++;   /* upper-left   */
    if (prev  & (1 << j))                         count++;   /* above        */
    if (j < WIDTH - 1 && (prev & (1 << (j + 1)))) count++;   /* upper-right */

    if (j > 0 && (curr  & (1 << (j - 1)))) count++;   /* left  */
    if (j < WIDTH - 1 && (curr & (1 << (j + 1)))) count++;   /* right */

    if (j > 0 && (next  & (1 << (j - 1)))) count++;   /* lower-left   */
    if (next  & (1 << j))                         count++;   /* below        */
    if (j < WIDTH - 1 && (next & (1 << (j + 1)))) count++;   /* lower-right */

    return count;
}

/* Return true if all 1‑cells in 'curr' satisfy the 0/3‑neighbor rule
   with neighboring rows 'prev' and 'next'. */
static inline bool valid_row(int prev, int curr, int next) {
    for (int j = 0; j < WIDTH; ++j) {
        if (curr & (1 << j)) {            /* this cell is a 1 */
            int c = count_neighbors(prev, curr, next, j);
            if (c != 0 && c != 3) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    /* n == 0 is outside the sequence offset; return 0 in that case. */
    if (n == 0) return 0;

    /* Precompute transition masks once. */
    static uint32_t trans[32][32];
    static bool table_built = false;
    if (!table_built) {
        for (int prev = 0; prev < PAT; ++prev) {
            for (int curr = 0; curr < PAT; ++curr) {
                uint32_t mask = 0;
                for (int next = 0; next < PAT; ++next) {
                    if (valid_row(prev, curr, next))
                        mask |= 1u << next;
                }
                trans[prev][curr] = mask;
            }
        }
        table_built = true;
    }

    /* Special case n == 1 : count patterns valid with both surrounding rows zero. */
    if (n == 1) {
        seq_t ans = 0;
        for (int curr = 0; curr < PAT; ++curr) {
            if (valid_row(0, curr, 0))
                ans += 1;
        }
        return ans;
    }

    /* DP arrays for two consecutive rows.  dp[prev][curr] counts ways for processed rows. */
    seq_t dp[32][32];
    seq_t newdp[32][32];
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            dp[i][j] = 0;

    /* Base: first row (row 1).  Row 0 is implicitly all zeros. */
    for (int curr = 0; curr < PAT; ++curr)
        dp[0][curr] = 1;

    /* Process rows 2 .. n. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < PAT; ++prev) {
            for (int curr = 0; curr < PAT; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                uint32_t mask = trans[prev][curr];
                while (mask) {
                    int next = __builtin_ctz(mask);
                    mask &= mask - 1;
                    newdp[curr][next] += val;
                }
            }
        }

        /* Copy newdp into dp for next iteration. */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final validation of the last row (row n) with virtual zero row below. */
    seq_t result = 0;
    for (int prev = 0; prev < PAT; ++prev) {
        for (int curr = 0; curr < PAT; ++curr) {
            if (valid_row(prev, curr, 0))
                result += dp[prev][curr];
        }
    }
    return result;
}
