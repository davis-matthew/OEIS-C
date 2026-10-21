/*
 * A296311  Number of nX6 0..1 arrays with each 1 adjacent to 3 or 6 king-move
 *          neighboring 1s.
 * https://oeis.org/A296311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by dynamic programming over rows of length 6.
 *            Each 1 must have 3 or 6 king‑move neighboring 1s. The DP keeps
 *            the last two rows and enumerates all possible next rows.
 *            Complexity: O(n·2¹²). All computations use the exactly‑typed
 *            types seq_t and idx_t supplied by the build. No hard‑...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint8_t neigh_lr[64][6];   /* left/right neighbours in a row   */
static uint8_t neigh_three[64][6]; /* left/centre/right neighbours in a row   */
static uint64_t trans_mask[4096]; /* mask of next rows allowed for a (prev,prev2) */

/* Precompute neighbour counts for every row pattern. */
static void init_precomp(void)
{
    static bool done = false;
    if (done) return;
    for (int r = 0; r < 64; ++r) {
        for (int j = 0; j < 6; ++j) {
            int lr = 0;
            if (j > 0 && (r & (1 << (j - 1)))) lr++;
            if (j < 5 && (r & (1 << (j + 1)))) lr++;
            neigh_lr[r][j] = (uint8_t)lr;

            int tr = 0;
            if (j > 0) tr += (r >> (j - 1)) & 1;
            tr += (r >> j) & 1;
            if (j < 5) tr += (r >> (j + 1)) & 1;
            neigh_three[r][j] = (uint8_t)tr;
        }
    }

    /* For each pair (prev,prev2) compute the bitmask of all next rows cur
       that make every 1 in prev satisfy the neighbour condition.   */
    for (int prev = 0; prev < 64; ++prev) {
        for (int prev2 = 0; prev2 < 64; ++prev2) {
            uint64_t mask = 0;
            for (int cur = 0; cur < 64; ++cur) {
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j) {
                    if ((prev >> j) & 1) {
                        int cnt = neigh_lr[prev][j]
                              + neigh_three[prev2][j]
                              + neigh_three[cur][j];
                        if (cnt != 3 && cnt != 6) ok = false;
                    }
                }
                if (ok) mask |= (1ULL << cur);
            }
            trans_mask[(prev << 6) | prev2] = mask;
        }
    }
    done = true;
}

seq_t a(idx_t n)
{
    init_precomp();

    /* dp[state]   state = (prev << 6) | prev2  */
    static seq_t dp[4096];
    static seq_t next[4096];
    for (int i = 0; i < 4096; ++i) dp[i] = 0;
    dp[0] = 1;                  /* (prev=0, prev2=0) */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 4096; ++i) next[i] = 0;
        for (int idx = 0; idx < 4096; ++idx) {
            seq_t ways = dp[idx];
            if (!ways) continue;
            uint64_t mask = trans_mask[idx];
            while (mask) {
                unsigned cur = __builtin_ctzll(mask);
                mask &= mask - 1;
                int new_idx = (cur << 6) | (idx >> 6);   /* (cur, prev) */
                next[new_idx] += ways;
            }
        }
        for (int i = 0; i < 4096; ++i) dp[i] = next[i];
    }

    /* Add dummy row of zeros to validate the last real row. */
    seq_t result = 0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t ways = dp[idx];
        if (!ways) continue;
        if (trans_mask[idx] & 1ULL)   /* bit 0 corresponds to cur = 0 */
            result += ways;
    }
    return result;
}
