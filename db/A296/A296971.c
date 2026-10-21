/*
 * A296971  Number of nX5 0..1 arrays with each 1 adjacent to 2, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   valid_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296971 – count of n × 5 binary arrays where each 1 has 2, 4
 * or 5 king‐move neighboring 1s.  The implementation uses dynamic
 * programming over rows.  For each pair of consecutive rows (prev,
 * curr) it precomputes all possible next rows that make the middle
 * row valid.  The DP runs in O(n) with a small constant factor
 * (≈10 transitions per state) and works for n up to 10 000 in far
 * below two seconds. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/valid_row.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    /* Pre‑compute transitions: for each (prev, curr) pair list all
     * next masks that make curr valid with prev and next.  The
     * tables are built only once. */
    static bool ready = false;
    static uint8_t trans_next[32 * 32][32];
    static uint8_t trans_cnt[32 * 32];
    if (!ready) {
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                int idx = (prev << 5) | curr;
                int cnt = 0;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (valid_row(prev, curr, (uint32_t)nxt))
                        trans_next[idx][cnt++] = (uint8_t)nxt;
                }
                trans_cnt[idx] = (uint8_t)cnt;
            }
        }
        ready = true;
    }

    /* Dynamic programming over rows.  dp[prev*32+curr] holds the
     * number of ways to reach the current state (prev row, curr
     * row).  Two arrays are used to alternate between successive
     * rows. */
    static seq_t dp[1024];
    static seq_t ndp[1024];
    seq_t *dp_ptr = dp;
    seq_t *ndp_ptr = ndp;

    /* initialise for the first row: previous row is all 0s. */
    for (int i = 0; i < 1024; ++i) dp_ptr[i] = 0;
    for (int curr = 0; curr < 32; ++curr) dp_ptr[curr] = 1;  /* prev = 0 */

    /* iterate over the remaining rows (n‑1 more rows). */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < 1024; ++i) ndp_ptr[i] = 0;
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp_ptr[(prev << 5) | curr];
                if (ways == 0) continue;
                int idx = (prev << 5) | curr;
                for (int k = 0; k < trans_cnt[idx]; ++k) {
                    uint8_t next = trans_next[idx][k];
                    ndp_ptr[(curr << 5) | next] += ways;
                }
            }
        }
        /* swap dp and ndp pointers */
        seq_t *tmp = dp_ptr;
        dp_ptr = ndp_ptr;
        ndp_ptr = tmp;
    }

    /* count only states where the last row (curr) is valid with a
     * zero row below. */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (valid_row(prev, curr, 0))
                ans += dp_ptr[(prev << 5) | curr];
        }
    }
    return ans;
}
