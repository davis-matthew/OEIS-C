/*
 * A295845  Number of n X 6 0..1 arrays with each 1 adjacent to 1 or 2
 *          king-move neighboring 1's.
 * https://oeis.org/A295845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by transfer‑matrix dynamic programming over three rows.
   The sequence counts n×6 0/1 arrays where every 1 has 1 or 2 king‑move
   neighboring 1’s.  No closed formula is known. */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* global static data to be built once */
static uint8_t  next_arr[64 * 64][64];
static uint8_t  next_cnt[64 * 64];
static bool     valid_last[64][64];
static unsigned int prevMaskForCol[6];
static unsigned int currMaskForCol[6];
static bool     precomputed = false;

/* help: test if a 1 at column c in cur has 1 or 2 king‑move neighbors
   among prev, cur (excluding centre), and next rows. */
static inline bool check_triplet(int prev, int curr, int next)
{
    /* iterate over all 1 bits in curr */
    for (int col = 0; col < 6; ++col) {
        if (!(curr & (1U << col)))
            continue;               /* only 0s impose no restriction */

        /* count neighbors */
        int cnt = 0;
        cnt += __builtin_popcount(prev & prevMaskForCol[col]);  /* upper neighbours */
        cnt += __builtin_popcount(curr & currMaskForCol[col]);  /* left/right neighbour in same row */
        cnt += __builtin_popcount(next & prevMaskForCol[col]);  /* lower neighbours – same mask as upper */

        if (cnt < 1 || cnt > 2)
            return false;
    }
    return true;
}

/* build all precomputed tables exactly once */
static void init_tables(void)
{
    /* precompute neighbor masks for the 6 columns */
    for (int j = 0; j < 6; ++j) {
        unsigned int mask = 0;
        for (int k = j - 1; k <= j + 1; ++k)
            if (k >= 0 && k < 6)
                mask |= 1U << k;
        prevMaskForCol[j] = mask;
        currMaskForCol[j] = ((j > 0) ? (1U << (j - 1)) : 0) |
                            ((j + 1 < 6) ? (1U << (j + 1)) : 0);
    }

    /* precompute transitions and ending validity */
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            int idx = prev * 64 + curr;
            uint8_t cnt = 0;
            for (int next = 0; next < 64; ++next) {
                if (check_triplet(prev, curr, next))
                    next_arr[idx][cnt++] = (uint8_t)next;
            }
            next_cnt[idx] = cnt;
        }
    }
    /* ending validity with trailing zero row */
    for (int prev = 0; prev < 64; ++prev)
        for (int last = 0; last < 64; ++last)
            valid_last[prev][last] = check_triplet(prev, last, 0);
    precomputed = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!precomputed)
        init_tables();

    static seq_t dp[64][64];
    static seq_t ndp[64][64];
    memset(dp, 0, sizeof(dp));

    /* initial state: previous row = 0, current row can be any pattern */
    for (int r = 0; r < 64; ++r)
        dp[0][r] = 1;

    /* iterate over the remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val)
                    continue;
                int idx = prev * 64 + curr;
                uint8_t cnt = next_cnt[idx];
                for (uint8_t t = 0; t < cnt; ++t) {
                    uint8_t nxt = next_arr[idx][t];
                    ndp[curr][nxt] += val;
                }
            }
        }
        /* swap dp and ndp */
        seq_t (*tmp)[64] = dp;
        memcpy(dp, ndp, sizeof(dp)); /* copy results back */
    }

    /* final answer: pick transitions that end with a trailing zero row */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int last = 0; last < 64; ++last) {
            if (dp[prev][last] && valid_last[prev][last])
                ans += dp[prev][last];
        }
    }
    return ans;
}
