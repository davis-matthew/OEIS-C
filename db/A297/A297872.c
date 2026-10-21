/*
 * A297872  Number of nX4 0..1 arrays with every element equal to 2, 3 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n×4 0..1 arrays with every element having 2, 3 or 5 king‑move
   adjacent cells of the same value (i.e., the same binary digit), upper‑left
   element zero.
   The implementation uses a transfer matrix on 4‑bit row states.  For a
   configuration to be valid, for each cell the number of adjacent cells that
   have the same value as the cell must be 2, 3 or 5.  The DP keeps the last
   two rows (prev, cur) and validates the middle row when the next row is
   chosen.  The tables valid_full, valid_missing_top, valid_missing_bottom
   and valid_both_missing encode the per‑row validity conditions. */

#include <stdbool.h>

#define WIDTH 4
#define ROWS  (1 << WIDTH)

static bool init = false;
static bool valid_full[ROWS][ROWS][ROWS];
static bool valid_missing_top[ROWS][ROWS];
static bool valid_missing_bottom[ROWS][ROWS];
static bool valid_both_missing[ROWS];

static inline bool in_set(int x) {
    return (x == 2 || x == 3 || x == 5);
}

static void compute_tables(void)
{
    /* Full rows: all neighbors exist. */
    for (int top = 0; top < ROWS; ++top)
        for (int mid = 0; mid < ROWS; ++mid)
            for (int bot = 0; bot < ROWS; ++bot) {
                bool ok = true;
                for (int j = 0; j < WIDTH; ++j) {
                    int midbit = (mid >> j) & 1;
                    int cnt = 0;
                    /* (-1,-1) */
                    if (j > 0 && ((top >> (j-1)) & 1) == midbit) ++cnt;
                    /* (-1,0) */
                    if (((top >> j) & 1) == midbit) ++cnt;
                    /* (-1,+1) */
                    if (j < WIDTH-1 && ((top >> (j+1)) & 1) == midbit) ++cnt;
                    /* (0,-1) */
                    if (j > 0 && ((mid >> (j-1)) & 1) == midbit) ++cnt;
                    /* (0,+1) */
                    if (j < WIDTH-1 && ((mid >> (j+1)) & 1) == midbit) ++cnt;
                    /* (+1,-1) */
                    if (j > 0 && ((bot >> (j-1)) & 1) == midbit) ++cnt;
                    /* (+1,0) */
                    if (((bot >> j) & 1) == midbit) ++cnt;
                    /* (+1,+1) */
                    if (j < WIDTH-1 && ((bot >> (j+1)) & 1) == midbit) ++cnt;
                    if (!in_set(cnt)) { ok = false; break; }
                }
                valid_full[top][mid][bot] = ok;
            }

    /* Missing top row (only mid and bot exist). */
    for (int mid = 0; mid < ROWS; ++mid)
        for (int bot = 0; bot < ROWS; ++bot) {
            bool ok = true;
            for (int j = 0; j < WIDTH; ++j) {
                int midbit = (mid >> j) & 1;
                int cnt = 0;
                /* (0,-1) */
                if (j > 0 && ((mid >> (j-1)) & 1) == midbit) ++cnt;
                /* (0,+1) */
                if (j < WIDTH-1 && ((mid >> (j+1)) & 1) == midbit) ++cnt;
                /* (+1,-1) */
                if (j > 0 && ((bot >> (j-1)) & 1) == midbit) ++cnt;
                /* (+1,0) */
                if (((bot >> j) & 1) == midbit) ++cnt;
                /* (+1,+1) */
                if (j < WIDTH-1 && ((bot >> (j+1)) & 1) == midbit) ++cnt;
                if (!in_set(cnt)) { ok = false; break; }
            }
            valid_missing_top[mid][bot] = ok;
        }

    /* Missing bottom row (only top and mid exist). */
    for (int top = 0; top < ROWS; ++top)
        for (int mid = 0; mid < ROWS; ++mid) {
            bool ok = true;
            for (int j = 0; j < WIDTH; ++j) {
                int midbit = (mid >> j) & 1;
                int cnt = 0;
                /* (-1,-1) */
                if (j > 0 && ((top >> (j-1)) & 1) == midbit) ++cnt;
                /* (-1,0) */
                if (((top >> j) & 1) == midbit) ++cnt;
                /* (-1,+1) */
                if (j < WIDTH-1 && ((top >> (j+1)) & 1) == midbit) ++cnt;
                /* (0,-1) */
                if (j > 0 && ((mid >> (j-1)) & 1) == midbit) ++cnt;
                /* (0,+1) */
                if (j < WIDTH-1 && ((mid >> (j+1)) & 1) == midbit) ++cnt;
                if (!in_set(cnt)) { ok = false; break; }
            }
            valid_missing_bottom[top][mid] = ok;
        }

    /* Only middle row exists (top and bottom missing). */
    for (int mid = 0; mid < ROWS; ++mid) {
        bool ok = true;
        for (int j = 0; j < WIDTH; ++j) {
            int midbit = (mid >> j) & 1;
            int cnt = 0;
            /* (0,-1) */
            if (j > 0 && ((mid >> (j-1)) & 1) == midbit) ++cnt;
            /* (0,+1) */
            if (j < WIDTH-1 && ((mid >> (j+1)) & 1) == midbit) ++cnt;
            if (!in_set(cnt)) { ok = false; break; }
        }
        valid_both_missing[mid] = ok;
    }
}

seq_t a(idx_t n)
{
    if (!init) { compute_tables(); init = true; }

    if (n == 0) return 0;           /* offset at 1, but guard for safety */

    /* Bases: n = 1 -> only middle row exists. */
    if (n == 1) {
        seq_t ans = 0;
        for (int mid = 0; mid < ROWS; ++mid) {
            if (!(mid & 1)) continue;              /* upper‑left zero */
            if (valid_both_missing[mid]) ans += 1;
        }
        return ans;
    }

    /* For n >= 2 we use DP with states [prev][cur]. */
    static seq_t dp[ROWS][ROWS];
    static seq_t nxt[ROWS][ROWS];

    /* Initialise dp for first two rows, validating the first row. */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp[i][j] = 0;

    for (int first = 0; first < ROWS; ++first) {
        if (first & 1) continue;                   /* upper‑left zero */
        for (int second = 0; second < ROWS; ++second) {
            if (valid_missing_top[first][second])
                dp[first][second] = 1;
        }
    }

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                nxt[i][j] = 0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (int nxtrow = 0; nxtrow < ROWS; ++nxtrow) {
                    if (valid_full[prev][cur][nxtrow]) {
                        nxt[cur][nxtrow] += ways;
                    }
                }
            }
        }
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* Final state: validate the last row with missing bottom. */
    seq_t ans = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (valid_missing_bottom[prev][cur]) ans += ways;
        }
    }
    return ans;
}
