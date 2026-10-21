/*
 * A279524  Number of nX3 0..2 arrays with no element equal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute A279524, the number of n×3 arrays with values 0..2, such
 *            that no element equals the strict majority of its horizontal and
 *            vertical neighbors, except for exactly one element. Values must
 *            be introduced in order 0,1,2 strictly upwards. The
 *            implementation uses dynamic programming over rows, t...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define PAT 27          /* 3^3 possible rows (3 columns) */
#define SENT 27         /* sentinel for 'no up/down neighbor' */

/* row_val[row][col] : 0..2 digit of a row pattern */
static uint8_t row_val[PAT][3];
/* row_bad[up][row][down] gives number of cells in 'row' that
   violate the majority rule when 'up' and 'down' neighbors exist.
   up==SENT or down==SENT denotes missing neighbor. */
static uint8_t row_bad[SENT+1][PAT][SENT+1];

/* pattern_start_valid[row] : true iff the row starts with 0 and
   respects the introduction order starting from an empty grid. */
static bool pattern_start_valid[PAT];
/* pattern_start_max[row]   : the maximum value seen after reading the row
   when starting from an empty grid (and starting with 0). */
static uint8_t pattern_start_max[PAT];

/* For a given starting maximum value 'start' (0..2), precompute
   whether a row is valid and what the new maximum will be. */
static bool pattern_valid_start[SENT+1][PAT];
static uint8_t pattern_newmax[SENT+1][PAT];

static void init_tables(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* compute row_val */
    for (int p = 0; p < PAT; ++p) {
        int tmp = p;
        for (int c = 0; c < 3; ++c) {
            row_val[p][c] = tmp % 3;
            tmp /= 3;
        }
    }

    /* compute row_bad for all up/down combinations */
    for (int up = 0; up <= SENT; ++up) {
        for (int row = 0; row < PAT; ++row) {
            for (int down = 0; down <= SENT; ++down) {
                uint8_t bad = 0;
                for (int c = 0; c < 3; ++c) {
                    int own = row_val[row][c];
                    int total = 0;
                    int owncnt = 0;
                    /* left neighbor */
                    if (c > 0) {
                        int v = row_val[row][c - 1];
                        ++total;
                        if (v == own) ++owncnt;
                    }
                    /* right neighbor */
                    if (c < 2) {
                        int v = row_val[row][c + 1];
                        ++total;
                        if (v == own) ++owncnt;
                    }
                    /* up neighbor */
                    if (up < PAT) {
                        int v = row_val[up][c];
                        ++total;
                        if (v == own) ++owncnt;
                    }
                    /* down neighbor */
                    if (down < PAT) {
                        int v = row_val[down][c];
                        ++total;
                        if (v == own) ++owncnt;
                    }
                    if (owncnt * 2 > total) ++bad;
                }
                row_bad[up][row][down] = bad;
            }
        }
    }

    /* compute pattern_start_valid and pattern_start_max */
    for (int r = 0; r < PAT; ++r) {
        if (row_val[r][0] != 0) {          /* first cell must be 0 */
            pattern_start_valid[r] = false;
            continue;
        }
        bool ok = true;
        int max = 0;                       /* after first cell */
        for (int c = 1; c < 3; ++c) {
            int v = row_val[r][c];
            if (v > max + 1) { ok = false; break; }
            if (v == max + 1 && max + 1 <= 2) max = max + 1;
        }
        pattern_start_valid[r] = ok;
        pattern_start_max[r] = (uint8_t)max;
    }

    /* compute pattern_valid_start and pattern_newmax for start max 0..2 */
    for (int start = 0; start <= 2; ++start) {
        for (int r = 0; r < PAT; ++r) {
            bool ok = true;
            int max = start;
            for (int c = 0; c < 3; ++c) {
                int v = row_val[r][c];
                if (v > max + 1) { ok = false; break; }
                if (v == max + 1 && max + 1 <= 2) max = max + 1;
            }
            pattern_valid_start[start][r] = ok;
            pattern_newmax[start][r] = (uint8_t)max;
        }
    }
}

/* main DP routine */
static seq_t compute_a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;

    /* dp[prev][curr][max][bad] */
    static seq_t dp[SENT+1][PAT][3][2];
    static seq_t ndp[SENT+1][PAT][3][2];

    /* clear dp and initialize with first row patterns */
    for (int i = 0; i <= SENT; ++i)
        for (int j = 0; j < PAT; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k][0] = dp[i][j][k][1] = 0;

    for (int r = 0; r < PAT; ++r) {
        if (!pattern_start_valid[r]) continue;
        int mm = pattern_start_max[r];
        dp[SENT][r][mm][0] += (seq_t)1;
    }

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* clear ndp */
        for (int i = 0; i <= SENT; ++i)
            for (int j = 0; j < PAT; ++j)
                for (int k = 0; k < 3; ++k)
                    ndp[i][j][k][0] = ndp[i][j][k][1] = 0;

        for (int prev = 0; prev <= SENT; ++prev) {
            for (int curr = 0; curr < PAT; ++curr) {
                for (int maxcur = 0; maxcur < 3; ++maxcur) {
                    for (int bad = 0; bad <= 1; ++bad) {
                        seq_t ways = dp[prev][curr][maxcur][bad];
                        if (!ways) continue;
                        for (int nxt = 0; nxt < PAT; ++nxt) {
                            if (!pattern_valid_start[maxcur][nxt]) continue;
                            int newmax = pattern_newmax[maxcur][nxt];
                            uint8_t badCurr = row_bad[prev][curr][nxt];
                            int nb = bad + badCurr;
                            if (nb <= 1)
                                ndp[curr][nxt][newmax][nb] += ways;
                        }
                    }
                }
            }
        }
        /* copy ndp back to dp */
        for (int i = 0; i <= SENT; ++i)
            for (int j = 0; j < PAT; ++j)
                for (int k = 0; k < 3; ++k)
                    for (int b = 0; b <= 1; ++b)
                        dp[i][j][k][b] = ndp[i][j][k][b];
    }

    /* finalization: count last row's violations */
    seq_t ans = 0;
    for (int prev = 0; prev <= SENT; ++prev) {
        for (int curr = 0; curr < PAT; ++curr) {
            for (int maxcur = 0; maxcur < 3; ++maxcur) {
                for (int bad = 0; bad <= 1; ++bad) {
                    seq_t ways = dp[prev][curr][maxcur][bad];
                    if (!ways) continue;
                    uint8_t badLast = row_bad[prev][curr][SENT];
                    if (bad + badLast == 1)
                        ans += ways;
                }
            }
        }
    }
    return ans;
}

/* external interface required by the build */
seq_t a(idx_t n)
{
    return compute_a(n);
}
