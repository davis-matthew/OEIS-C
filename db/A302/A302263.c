/*
 * A302263  Number of nX6 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302263
 *  a(n) = number of n×6 binary arrays with the property that for every cell
 *  the number of horizontally, diagonally or antidiagonally adjacent cells
 *  having the same value is at most 3; the upper‑left cell is fixed to 0.
 *
 *  The algorithm uses dynamic programming on pairs of consecutive rows
 *  (a 64×64 state space).  Transition from state (prev,cur) to
 *  (cur,nxt) is allowed iff the middle row cur satisfies the adjacency
 *  restriction with the rows prev (above) and nxt (below).  The DP works
 *  in O(n·(64^3)) time with a precomputed mask of valid nxt rows for
 *  each (prev,cur) pair, which reduces the inner loop to the actual
 *  number of admissible transitions (typically < 32).  The total
 *  complexity is well below the allowed time for n up to 10000.
 *
 *  All arithmetic is performed in the generic integer type `seq_t`; no
 *  overflows occur because intermediate values never exceed the size of
 *  the final answer for the given n.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* helper function: test whether row 'cur' satisfies the adjacency constraint
 * when the row above is 'prev' (valid if hasPrev==true) and the row below
 * is 'nxt'   (valid if hasNxt==true).  Neighbours that are outside the
 * array (col < 0 or col > 5) are ignored.
 */
static bool valid_row(uint64_t prev, uint64_t cur, uint64_t nxt,
                      bool hasPrev, bool hasNxt)
{
    for (int col = 0; col < 6; ++col) {
        int v = (int)((cur >> col) & 1U);
        int cnt = 0;

        /* left and right neighbours in the same row */
        if (col > 0   && ((cur >> (col - 1)) & 1U) == v) ++cnt;
        if (col < 5   && ((cur >> (col + 1)) & 1U) == v) ++cnt;

        /* above neighbours (if any) */
        if (hasPrev) {
            if (col > 0   && ((prev >> (col - 1)) & 1U) == v) ++cnt;
            if (col < 5   && ((prev >> (col + 1)) & 1U) == v) ++cnt;
        }

        /* below neighbours (if any) */
        if (hasNxt) {
            if (col > 0   && ((nxt >> (col - 1)) & 1U) == v) ++cnt;
            if (col < 5   && ((nxt >> (col + 1)) & 1U) == v) ++cnt;
        }

        if (cnt > 3) return false;
    }
    return true;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    /* Handle offset: the first term corresponds to n = 1 */
    if (n < 1) return 0;

    /* Precomputation of transition masks and end‑row checks on first call */
    static bool initialized = false;
    static uint64_t next_mask[64 * 64];
    static bool last_ok[64 * 64];
    static bool start_ok[64];
    if (!initialized) {
        for (uint64_t prev = 0; prev < 64; ++prev) {
            for (uint64_t cur = 0; cur < 64; ++cur) {
                uint64_t mask = 0;
                for (uint64_t nxt = 0; nxt < 64; ++nxt) {
                    if (valid_row(prev, cur, nxt, true, true))
                        mask |= 1ULL << nxt;
                }
                next_mask[(prev << 6) | cur] = mask;
                last_ok[(prev << 6) | cur] = valid_row(prev, cur, 0, true, false);
            }
        }
        for (uint64_t r = 0; r < 64; ++r) {
            start_ok[r] = valid_row(0, r, 0, false, false) && ((r & 1U) == 0);
        }
        initialized = true;
    }

    /* Quick case n == 1 */
    if (n == 1U) {
        seq_t ans = 0;
        for (uint64_t r = 0; r < 64; ++r)
            if (start_ok[r]) ++ans;
        return ans;
    }

    /* Allocate DP arrays */
    seq_t *dp = (seq_t *)calloc(64 * 64, sizeof(seq_t));
    seq_t *nextdp = (seq_t *)calloc(64 * 64, sizeof(seq_t));
    if (!dp || !nextdp) { free(dp); free(nextdp); return 0; }

    /* Initialize DP with the first two rows */
    for (uint64_t r0 = 0; r0 < 64; ++r0) {
        if (!((r0 & 1U) == 0)) continue;          /* upper left must be 0 */
        for (uint64_t r1 = 0; r1 < 64; ++r1) {
            if (valid_row(0, r0, r1, false, true))
                dp[(r0 << 6) | r1] = 1;
        }
    }

    /* Iterate rows 3 through n */
    for (idx_t row = 2; row < n; ++row) {
        memset(nextdp, 0, sizeof(seq_t) * 64 * 64);
        for (uint64_t s = 0; s < 64U * 64U; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            uint64_t prev = s >> 6;
            uint64_t cur  = s & 63U;
            uint64_t mask = next_mask[s];
            while (mask) {
                int nxt = __builtin_ctzll(mask);
                mask &= mask - 1;
                nextdp[(cur << 6) | nxt] += val;
            }
        }
        /* swap dp and nextdp */
        seq_t *tmp = dp; dp = nextdp; nextdp = tmp;
    }

    /* Sum over final states where the last row satisfies the constraint */
    seq_t ans = 0;
    for (uint64_t s = 0; s < 64U * 64U; ++s) {
        if (dp[s] && last_ok[s])
            ans += dp[s];
    }

    free(dp); free(nextdp);
    return ans;
}
