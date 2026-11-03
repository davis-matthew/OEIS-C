/*
 * A299364  Number of nX6 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 196 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of n×6 binary arrays such that for every
 * cell the number of same‑valued king‑move neighbours is in {0,2,3,4,5,8}.
 * Upper‑left cell must be 0.  The algorithm uses dynamic programming
 * over row patterns (64 possible 6‑bit rows); each state is a pair of
 * consecutive rows.  Precomputation of transitions is done once.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: check if 'curr' row satisfies the condition when the
 * upper row is 'prev' (hasPrev flag indicates whether prev exists)
 * and the lower row is 'next' (hasNext flag indicates; whenever a
 * row does not exist it is ignored). */
static bool row_valid_middle(uint8_t prev, uint8_t curr, uint8_t next,
                             bool hasPrev, bool hasNext)
{
    /* 0..5 columns */
    for (int col = 0; col < 6; ++col) {
        uint8_t val = (curr >> col) & 1U;
        int cnt = 0;

        /* left/right on same row */
        if (col > 0)
            cnt += (((curr >> (col - 1)) & 1U) == val);
        if (col < 5)
            cnt += (((curr >> (col + 1)) & 1U) == val);

        /* neighbours on previous row */
        if (hasPrev) {
            cnt += (((prev >> col) & 1U) == val);
            if (col > 0)
                cnt += (((prev >> (col - 1)) & 1U) == val);
            if (col < 5)
                cnt += (((prev >> (col + 1)) & 1U) == val);
        }

        /* neighbours on next row */
        if (hasNext) {
            cnt += (((next >> col) & 1U) == val);
            if (col > 0)
                cnt += (((next >> (col - 1)) & 1U) == val);
            if (col < 5)
                cnt += (((next >> (col + 1)) & 1U) == val);
        }

        /* allowed counts are 0,2,3,4,5,8 */
        if (cnt == 1 || cnt == 6 || cnt == 7) return false;
    }
    return true;
}

/* precomputation status */
static bool _precomp_done = false;
static bool start_ok[64][64];
static bool bottom_ok[64][64];
static uint64_t nextMask[64][64];

/* precomputation routine */
static void _precompute(void)
{
    if (_precomp_done) return;
    uint8_t dummy = 0; /* value ignored when !hasPrev or !hasNext */
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            /* top row check: no above, have below */
            start_ok[prev][curr] = row_valid_middle(dummy, prev, curr, false, true);

            /* bottom row check: have above, no below */
            bottom_ok[prev][curr] = row_valid_middle(prev, curr, dummy, true, false);

            /* interior transition: have both neighbours */
            uint64_t m = 0;
            for (int next = 0; next < 64; ++next) {
                if (row_valid_middle(prev, curr, next, true, true))
                    m |= 1ULL << next;
            }
            nextMask[prev][curr] = m;
        }
    }
    _precomp_done = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    _precompute();

    if (n == 0) return 0; /* not used, but safe */

    /* handle n = 1 separately (single row: no above/below neighbors) */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < 64; ++row) {
            if ((row & 1) != 0) continue;                 /* upper left must be 0 */
            if (row_valid_middle(0, row, 0, false, false))
                ++total;
        }
        return total;
    }

    /* DP arrays: dp[prev][curr] */
    static seq_t dp[64][64];
    static seq_t nextdp[64][64];
    /* initialize to zero */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* initial states: two rows, top row bit0 must be 0 */
    for (int row0 = 0; row0 < 64; ++row0) {
        if ((row0 & 1) != 0) continue;         /* upper left zero */
        for (int row1 = 0; row1 < 64; ++row1) {
            if (start_ok[row0][row1]) {
                ++dp[row0][row1];
            }
        }
    }

    /* iterate rows 2 .. n-1 (total n rows, already placed 2) */
    for (idx_t step = 1; step <= n - 2; ++step) {
        /* zero nextdp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                nextdp[i][j] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt) continue;
                uint64_t m = nextMask[prev][curr];
                while (m) {
                    int nxt = __builtin_ctzll(m);
                    nextdp[curr][nxt] += cnt;
                    m &= m - 1;
                }
            }
        }
        /* swap dp and nextdp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* final answer: sum of states where bottom row is valid */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            if (dp[prev][curr] && bottom_ok[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
