/*
 * A299317  Number of nX4 0..1 arrays with every element equal to 0, 3, 4, 5,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299317: Count of n×4 binary arrays with special king‑move
   adjacency constraints.  The recurrence is implemented by dynamic
   programming over the 4‑bit rows of the array.  For each row we
   pre‑compute which pairs of consecutive rows are compatible (by
   checking that the upper row satisfies the rule when its lower
   neighbour is the next row).  The DP then counts all possible
   sequences of rows, applying the initial and final row constraints.
   The function works for any integer type seq_t (e.g. uint8_t…uint64_t,
   unsigned __int128, or an arbitrary‑precision class).  */

#include <stdbool.h>
#include <stdint.h>

static bool allowed_count[9] = {
    /* 0, 1, 2, 3, 4, 5, 6, 7, 8 */
    true,  false, false, true, true, true, true, true, false
};

static bool first_ok[16][16];
static bool mid_ok[16][16][16];
static bool last_ok[16][16];
static bool tables_built = false;

/* Check whether a 4‑bit row can be the first row of an array
   (no upper neighbour).  'top' is the current row, 'bottom' is the
   row that follows it.  */
static bool check_first_row(uint8_t top, uint8_t bottom)
{
    for (int col = 0; col < 4; ++col) {
        int v = (top >> col) & 1;
        int cnt = 0;

        /* left/right in the same row */
        if (col > 0   && ((top >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((top >> (col + 1)) & 1) == v) ++cnt;

        /* down neighbours */
        if (((bottom >> col) & 1) == v) ++cnt;
        if (col > 0   && ((bottom >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((bottom >> (col + 1)) & 1) == v) ++cnt;

        if (!allowed_count[(cnt <= 8) ? cnt : 8]) return false;
    }
    return true;
}

/* Check whether a 4‑bit row can be the last row of an array
   (no lower neighbour).  'top' is the row above it, 'cur' is the
   last row.  */
static bool check_last_row(uint8_t top, uint8_t cur)
{
    for (int col = 0; col < 4; ++col) {
        int v = (cur >> col) & 1;
        int cnt = 0;

        /* left/right in the same row */
        if (col > 0   && ((cur >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((cur >> (col + 1)) & 1) == v) ++cnt;

        /* up neighbours */
        if (((top >> col) & 1) == v) ++cnt;
        if (col > 0   && ((top >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((top >> (col + 1)) & 1) == v) ++cnt;

        if (!allowed_count[(cnt <= 8) ? cnt : 8]) return false;
    }
    return true;
}

/* Check whether a 4‑bit row can be a middle row (has both upper and
   lower neighbours).  'up' is the row above, 'cur' is the current,
   'down' is the row below.  */
static bool check_mid_row(uint8_t up, uint8_t cur, uint8_t down)
{
    for (int col = 0; col < 4; ++col) {
        int v = (cur >> col) & 1;
        int cnt = 0;

        /* left/right in the same row */
        if (col > 0   && ((cur >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((cur >> (col + 1)) & 1) == v) ++cnt;

        /* up neighbours */
        if (((up >> col) & 1) == v) ++cnt;
        if (col > 0   && ((up >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((up >> (col + 1)) & 1) == v) ++cnt;

        /* down neighbours */
        if (((down >> col) & 1) == v) ++cnt;
        if (col > 0   && ((down >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((down >> (col + 1)) & 1) == v) ++cnt;

        if (!allowed_count[(cnt <= 8) ? cnt : 8]) return false;
    }
    return true;
}

/* Check whether a single 4‑bit row satisfies the rule without
   neighbours.  The upper‑left element must be zero.  */
static bool check_single_row(uint8_t mask)
{
    if (mask & 1) return false;          /* (row, col) = (0,0) must be 0 */

    for (int col = 0; col < 4; ++col) {
        int v = (mask >> col) & 1;
        int cnt = 0;

        /* left/right neighbours only */
        if (col > 0   && ((mask >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((mask >> (col + 1)) & 1) == v) ++cnt;

        if (!allowed_count[(cnt <= 8) ? cnt : 8]) return false;
    }
    return true;
}

/* Build transition tables once.  */
static void build_tables(void)
{
    for (int a = 0; a < 16; ++a) {
        if (a & 1) continue;                /* upper‑left zero restriction */
        for (int b = 0; b < 16; ++b) {
            first_ok[a][b] = check_first_row((uint8_t)a, (uint8_t)b);
        }
    }

    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            last_ok[a][b] = check_last_row((uint8_t)a, (uint8_t)b);
        }
    }

    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                mid_ok[a][b][c] = check_mid_row((uint8_t)a,
                                                (uint8_t)b,
                                                (uint8_t)c);
            }
        }
    }
    tables_built = true;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (!tables_built) build_tables();

    if (n == 0) return 0;          /* unspecified, but safe */

    /* n==1 is a single row */
    if (n == 1) {
        seq_t ans = 0;
        for (int m = 0; m < 16; ++m)
            if (check_single_row((uint8_t)m))
                ++ans;
        return ans;
    }

    /* DP arrays: dp[prev][cur] stores number of ways to finish up to
       the current row, where 'prev' is the second‑last row and
       'cur' the last row.  */
    seq_t dp[16][16] = {0};
    seq_t ndp[16][16] = {0};

    /* Initialise with the first two rows (top row has no upper neighbour). */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            if (first_ok[a][b])
                dp[a][b] = 1;

    /* Iterate over remaining rows (total rows = n). */
    for (idx_t r = 2; r < n; ++r) {      /* r=2 .. n-1 indexing rows 0..n-1 */
        /* zero ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t val = dp[prev][cur];
                if (!val) continue;
                for (int next = 0; next < 16; ++next) {
                    if (mid_ok[prev][cur][next])
                        ndp[cur][next] += val;
                }
            }
        }
        /* copy ndp -> dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all valid last row pairs. */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            if (last_ok[prev][cur])
                ans += dp[prev][cur];
        }
    }
    return ans;
}
