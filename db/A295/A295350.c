/*
 * A295350  Number of n X 6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 3 1's.
 * https://oeis.org/A295350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays in which every 1 has 0, 2, or 3
 *            adjacent 1's (horizontal or vertical). Computed by dynamic
 *            programming on rows (6 bits each)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Pre‑computed transition table:  for each pair (up,mid) of row
 * values the bitmask of all rows down that are compatible with
 * the middle row.  The table is generated on first use. */
static uint64_t trans_mask[64][64];
static bool trans_ready = false;

static inline bool valid_triplet(int up, int mid, int down)
{
    for (int c = 0; c < 6; ++c) {
        if ((mid >> c) & 1) {
            int upb   = (up   >> c) & 1;
            int downb = (down >> c) & 1;
            int horiz = 0;
            if (c > 0   && (mid >> (c-1)) & 1) horiz++;
            if (c < 5   && (mid >> (c+1)) & 1) horiz++;
            int neigh = upb + downb + horiz;
            if (neigh == 1 || neigh == 4) return false;
        }
    }
    return true;
}

static void prepare_trans(void)
{
    if (trans_ready) return;
    for (int up = 0; up < 64; ++up) {
        for (int mid = 0; mid < 64; ++mid) {
            uint64_t m = 0;
            for (int down = 0; down < 64; ++down) {
                if (valid_triplet(up, mid, down))
                    m |= 1ULL << down;
            }
            trans_mask[up][mid] = m;
        }
    }
    trans_ready = true;
}

#define ST 4096        /* 64 × 64 states */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset 1: a(1) is first term */
    prepare_trans();

#ifdef __cplusplus
    std::vector<seq_t> dp(ST), dp_next(ST);
#else
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * ST);
    seq_t *dp_next = (seq_t *)malloc(sizeof(seq_t) * ST);
    if (!dp || !dp_next) abort();
    for (size_t i = 0; i < ST; ++i) dp[i] = dp_next[i] = 0;
#endif

    /* first row (row 1) with dummy upper row 0 */
#ifdef __cplusplus
    std::fill(dp.begin(), dp.end(), (seq_t)0);
#else
    for (size_t i = 0; i < ST; ++i) dp[i] = 0;
#endif
    for (int r = 0; r < 64; ++r) {
        dp[0 * 64 + r] = 1;  /* up = 0, mid = r */
    }

    for (idx_t step = 1; step < n; ++step) {
#ifdef __cplusplus
        std::fill(dp_next.begin(), dp_next.end(), (seq_t)0);
#else
        for (size_t i = 0; i < ST; ++i) dp_next[i] = 0;
#endif
        for (int up = 0; up < 64; ++up) {
            int base = up * 64;
            for (int mid = 0; mid < 64; ++mid) {
                seq_t cnt = dp[base + mid];
                if (cnt == 0) continue;
                uint64_t m = trans_mask[up][mid];
                while (m) {
                    int next = __builtin_ctzll(m);
                    dp_next[(mid << 6) | next] += cnt;
                    m &= m - 1;   /* clear lowest set bit */
                }
            }
        }
#ifdef __cplusplus
        dp.swap(dp_next);
#else
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
#endif
    }

    seq_t ans = 0;
    for (int up = 0; up < 64; ++up) {
        int base = up * 64;
        for (int mid = 0; mid < 64; ++mid) {
            seq_t cnt = dp[base + mid];
            if (cnt == 0) continue;
            if (valid_triplet(up, mid, 0))
                ans += cnt;
        }
    }

#ifndef __cplusplus
    free(dp);
    free(dp_next);
#endif
    return ans;
}
