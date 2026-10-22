/*
 * A302372  Number of 6Xn 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   valid_center
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302372
 * A(1)=32, A(2)=10, A(3)=1430, A(4)=774, A(5)=49293, A(6)=330505, ...
 * Number of 6×n 0..1 arrays with every element equal to
 * 0,2,3 or 4 horizontally, diagonally or antidiagonally adjacent
 * elements, with upper left element zero.
 *
 * The function implements a dynamic programming over column masks.
 * Each column is represented by a 6‑bit mask (bit 0 = top row).
 * For a column C2 with neighbours C1 and C3 a cell (row r) has
 * up to six neighbours: (r,C1),(r-1,C1),(r+1,C1),(r,C3),(r-1,C3),(r+1,C3).
 * The number of neighbours equal to the cell's value must be
 * 0, 2, 3 or 4.  The above DP counts all length‑n arrays
 * satisfying this constraint and upper left zero.
 *
 * The implementation uses the following pre‑computations:
 *   - rights_cnt[l][c] : number of right masks that keep column c
 *     valid when it has neighbours l and r.
 *   - rights_set[l][c][k] : the actual right mask values for that pair.
 *   - maskRightFirst[c]   : bitset of right masks that keep column c
 *     valid when it has no left neighbour (first column).
 *   - maskLeftLast[c]     : bitset of left masks that keep column c
 *     valid when it has no right neighbour (last column).
 *
 * Complexity:  O(n·64³) ~ 2.6×10⁵ operations per column (worst case),
 * feasible for n up to 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/valid_center.h"

static bool init_done = false;
static uint8_t  rights_set[64][64][64];
static uint8_t  rights_cnt[64][64];
static uint64_t maskRightFirst[64];
static uint64_t maskLeftLast[64];

static inline void
precompute(void)
{
    if (init_done) return;

    for (int left = 0; left < 64; ++left) {
        for (int center = 0; center < 64; ++center) {
            uint8_t cnt = 0;
            for (int right = 0; right < 64; ++right) {
                if (valid_center(left, center, right, true, true))
                    rights_set[left][center][cnt++] = (uint8_t)right;
            }
            rights_cnt[left][center] = cnt;
        }
    }

    for (int center = 0; center < 64; ++center) {
        uint64_t m = 0;
        for (int right = 0; right < 64; ++right) {
            if (valid_center(0, center, right, false, true))
                m |= 1ULL << right;
        }
        maskRightFirst[center] = m;
    }

    for (int center = 0; center < 64; ++center) {
        uint64_t m = 0;
        for (int left = 0; left < 64; ++left) {
            if (valid_center(left, center, 0, true, false))
                m |= 1ULL << left;
        }
        maskLeftLast[center] = m;
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 32;              /* any 6‑bit mask with top row zero */

    precompute();

    static seq_t dp[64][64];
    static seq_t nxt[64][64];
    memset(dp, 0, sizeof(dp));

    /* initial two columns: first column must satisfy its only neighbour rule */
    for (int first = 0; first < 64; ++first) {
        if ((first & 1) == 0) {                  /* upper‑left must be 0 */
            uint64_t firstMask = maskRightFirst[first];
            for (int second = 0; second < 64; ++second) {
                if ((firstMask >> second) & 1ULL)
                    dp[first][second] = 1;
            }
        }
    }

    /* DP over remaining columns */
    for (idx_t col = 2; col < n; ++col) {
        memset(nxt, 0, sizeof(nxt));
        for (int left = 0; left < 64; ++left) {
            for (int mid = 0; mid < 64; ++mid) {
                seq_t val = dp[left][mid];
                if (val == 0) continue;
                uint8_t cnt = rights_cnt[left][mid];
                for (int k = 0; k < cnt; ++k) {
                    int right = rights_set[left][mid][k];
                    nxt[mid][right] += val;
                }
            }
        }
        /* swap dp and nxt */
        memcpy(dp, nxt, sizeof(dp));
    }

    /* final column must satisfy its only left neighbour rule */
    seq_t ans = 0;
    for (int first = 0; first < 64; ++first) {
        for (int last = 0; last < 64; ++last) {
            seq_t val = dp[first][last];
            if (val == 0) continue;
            if (((maskLeftLast[last] >> first) & 1ULL) == 0) continue;
            ans += val;
        }
    }
    return ans;
}
