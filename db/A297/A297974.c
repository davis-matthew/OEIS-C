/*
 * A297974  Number of n X 4 0..1 arrays with every element equal to 2, 3, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over the 4‑column binary
 *            rows of an n×4 array. For each cell the number of same‑valued
 *            king‑move adjacent cells must be between 2 and 5 inclusive. The
 *            upper‑left cell is fixed to 0 (bit 0 of the first row is 0). The
 *            DP uses the 16 possible rows as states, p...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 16‑bit masks for the 4‑column rows (bit 0 = leftmost column). */
static bool  valid_mid[16][16][16];
static uint16_t trans_mask[16][16];    /* mask of next rows for a given (above,mid) */

static bool  valid_first[16][16];
static uint16_t first_mask[16];       /* mask of second rows for a given first row */

static bool  valid_last[16][16];
static uint16_t last_mask[16];        /* mask of last rows for a given previous row */

static bool  valid_single[16];        /* single row with no neighbours */

static bool tables_inited = false;

/* Helper: test validity of a single cell's same‑valued neighbors.
   has_above and has_below indicate whether the neighbouring row
   exists.  */
static bool row_valid(uint16_t row, uint16_t above, uint16_t below,
                      bool has_above, bool has_below)
{
    for (int col = 0; col < 4; ++col) {
        int val = (row >> col) & 1;
        int cnt = 0;

        /* left neighbour in same row */
        if (col > 0 && ((row >> (col - 1)) & 1) == val)
            ++cnt;
        /* right neighbour in same row */
        if (col < 3 && ((row >> (col + 1)) & 1) == val)
            ++cnt;

        if (has_above) {
            if (((above >> col) & 1) == val)
                ++cnt;
            if (col > 0 && (((above >> (col - 1)) & 1) == val))
                ++cnt;
            if (col < 3 && (((above >> (col + 1)) & 1) == val))
                ++cnt;
        }

        if (has_below) {
            if (((below >> col) & 1) == val)
                ++cnt;
            if (col > 0 && (((below >> (col - 1)) & 1) == val))
                ++cnt;
            if (col < 3 && (((below >> (col + 1)) & 1) == val))
                ++cnt;
        }

        if (cnt < 2 || cnt > 5)
            return false;
    }
    return true;
}

/* Pre‑compute all tables.  */
static void init_tables(void)
{
    if (tables_inited)
        return;
    tables_inited = true;

    /* Single row (no neighbours) */
    for (int row = 0; row < 16; ++row)
        valid_single[row] = row_valid(row, 0, 0, false, false);

    /* First row: no above, below exists. */
    for (int row = 0; row < 16; ++row) {
        uint16_t mask = 0;
        for (int below = 0; below < 16; ++below) {
            if (row_valid(row, 0, below, false, true)) {
                valid_first[row][below] = true;
                mask |= (1u << below);
            } else
                valid_first[row][below] = false;
        }
        first_mask[row] = mask;
    }

    /* Last row: above exists, no below. */
    for (int above = 0; above < 16; ++above) {
        uint16_t mask = 0;
        for (int row = 0; row < 16; ++row) {
            if (row_valid(row, above, 0, true, false)) {
                valid_last[above][row] = true;
                mask |= (1u << row);
            } else
                valid_last[above][row] = false;
        }
        last_mask[above] = mask;
    }

    /* Middle rows: both above and below exist. */
    for (int above = 0; above < 16; ++above)
        for (int mid = 0; mid < 16; ++mid) {
            uint16_t mask = 0;
            for (int below = 0; below < 16; ++below) {
                if (row_valid(mid, above, below, true, true)) {
                    valid_mid[above][mid][below] = true;
                    mask |= (1u << below);
                } else
                    valid_mid[above][mid][below] = false;
            }
            trans_mask[above][mid] = mask;
        }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset starts at 1 */

    init_tables();

    /* Cache for DP states.  */
    static seq_t dp_cur[16][16];
    static seq_t dp_next[16][16];

    /* --- n == 1 ----------------------------------------------------- */
    if (n == 1) {
        seq_t cnt = 0;
        for (int r = 0; r < 16; ++r)
            if ((r & 1) == 0 && valid_single[r])
                cnt += (seq_t)1;
        return cnt;
    }

    /* --- n == 2 ----------------------------------------------- */
    if (n == 2) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < 16; ++r0) {
            if ((r0 & 1) == 0) {
                uint16_t inter = first_mask[r0] & last_mask[r0];
                ans += (seq_t)__builtin_popcount(inter);
            }
        }
        return ans;
    }

    /* --- n >= 3 ----------------------------------------------- */
    /* Initialise dp_prev for rows 1 and 2.  */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp_cur[i][j] = (seq_t)0;

    for (int r0 = 0; r0 < 16; ++r0) {
        if ((r0 & 1) != 0)
            continue;            /* upper-left zero required */
        uint16_t mask = first_mask[r0];
        for (int r1 = 0; r1 < 16; ++r1)
            if (mask & (1u << r1))
                dp_cur[r0][r1] = (seq_t)1;
    }

    /* Iterate rows 3 .. n.  */
    for (idx_t rowIdx = 3; rowIdx <= n; ++rowIdx) {
        /* Zero dp_next. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_next[i][j] = (seq_t)0;

        /* Transitions. */
        for (int prev = 0; prev < 16; ++prev) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t cnt = dp_cur[prev][cur];
                if (cnt == (seq_t)0)
                    continue;
                uint16_t m = trans_mask[prev][cur];
                for (int nxt = 0; nxt < 16; ++nxt)
                    if (m & (1u << nxt))
                        dp_next[cur][nxt] += cnt;
            }
        }
        /* Swap dp arrays. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Sum over valid last row. */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            if (dp_cur[prev][cur] == (seq_t)0)
                continue;
            if (last_mask[prev] & (1u << cur))
                ans += dp_cur[prev][cur];
        }
    }
    return ans;
}
