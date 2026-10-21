/*
 * A297903  Number of nX4 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A297903 – number of n×4 binary arrays with every element
   equal to 0, 1, 3, or 4 king‑move adjacent equal elements, upper‑left
   element fixed to 0.  The implementation uses dynamic programming
   over rows.  It returns a(0)=0, a(1)=5, a(2)=13, … as required. */

#include <stddef.h>
#include <stdbool.h>

/* ---------- constant tables ----------------------------------------------*/

static bool initialized = false;

static bool allowed[9];                          /* allowed same‑neighbour counts */
static bool top_ok[16][16];                      /* a is first, b second */
static bool mid_ok[16][16][16];                  /* middle row check */
static bool bot_ok[16][16];                      /* previous, current */
static int  next_mid_count[256];                 /* number of valid third rows */
static int  next_mid_rows[256][16];              /* list of valid third rows */

/* ---------- helpers -------------------------------------------------------*/

static inline bool cell_ok(int val, int same)
{
    return allowed[same];
}

/* single row – no upper/lower neighbours */
static bool check_single(int row)
{
    for (int j = 0; j < 4; ++j) {
        int val  = (row >> j) & 1;
        int same = 0;
        if (j > 0   && ((row >> (j-1)) & 1) == val) ++same;
        if (j < 3   && ((row >> (j+1)) & 1) == val) ++same;
        if (!cell_ok(val, same)) return false;
    }
    return true;
}

/* top row – no upper neighbours */
static bool check_top(int a, int b)
{
    for (int j = 0; j < 4; ++j) {
        int val  = (a >> j) & 1;
        int same = 0;
        if (j > 0   && ((a >> (j-1)) & 1) == val) ++same;
        if (j < 3   && ((a >> (j+1)) & 1) == val) ++same;
        if (j > 0   && ((b >> (j-1)) & 1) == val) ++same;
        if (((b >> j) & 1) == val) ++same;
        if (j < 3   && ((b >> (j+1)) & 1) == val) ++same;
        if (!cell_ok(val, same)) return false;
    }
    return true;
}

/* middle row – has upper and lower neighbours */
static bool check_middle(int prev, int curr, int next)
{
    for (int j = 0; j < 4; ++j) {
        int val  = (curr >> j) & 1;
        int same = 0;
        if (j > 0   && ((curr >> (j-1)) & 1) == val) ++same;
        if (j < 3   && ((curr >> (j+1)) & 1) == val) ++same;
        if (j > 0   && ((prev >> (j-1)) & 1) == val) ++same;
        if (((prev >> j) & 1) == val) ++same;
        if (j < 3   && ((prev >> (j+1)) & 1) == val) ++same;
        if (j > 0   && ((next >> (j-1)) & 1) == val) ++same;
        if (((next >> j) & 1) == val) ++same;
        if (j < 3   && ((next >> (j+1)) & 1) == val) ++same;
        if (!cell_ok(val, same)) return false;
    }
    return true;
}

/* bottom row – no lower neighbours */
static bool check_bottom(int prev, int curr)
{
    for (int j = 0; j < 4; ++j) {
        int val  = (curr >> j) & 1;
        int same = 0;
        if (j > 0   && ((curr >> (j-1)) & 1) == val) ++same;
        if (j < 3   && ((curr >> (j+1)) & 1) == val) ++same;
        if (j > 0   && ((prev >> (j-1)) & 1) == val) ++same;
        if (((prev >> j) & 1) == val) ++same;
        if (j < 3   && ((prev >> (j+1)) & 1) == val) ++same;
        if (!cell_ok(val, same)) return false;
    }
    return true;
}

/* pre‑compute all transition tables */
static void init_tables(void)
{
    for (int i = 0; i < 9; ++i) allowed[i] = false;
    allowed[0] = true;
    allowed[1] = true;
    allowed[3] = true;
    allowed[4] = true;

    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            top_ok[a][b] = check_top(a, b);
            bot_ok[a][b] = check_bottom(a, b);
            for (int c = 0; c < 16; ++c) {
                mid_ok[a][b][c] = check_middle(a, b, c);
            }
        }
    }

    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            int pair = (a << 4) | b;
            int k = 0;
            for (int c = 0; c < 16; ++c) {
                if (mid_ok[a][b][c]) {
                    next_mid_rows[pair][k++] = c;
                }
            }
            next_mid_count[pair] = k;
        }
    }
    initialized = true;
}

/* ---------- required function --------------------------------------------*/

seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    if (n == 0) return 0;           /* below offset */

    /* n == 1 : single row */
    if (n == 1) {
        seq_t cnt = 0;
        for (int row = 0; row < 16; ++row) {
            if ((row & 1) != 0) continue;   /* upper‑left must be 0 */
            if (check_single(row)) {
                cnt += 1;
            }
        }
        return cnt;
    }

    /* DP arrays: dp[prev][curr] holds number of ways up to current row */
    seq_t dp[16][16]   = {0};
    seq_t ndp[16][16]  = {0};

    /* initialise with first two rows */
    for (int a = 0; a < 16; ++a) {
        if ((a & 1) != 0) continue;           /* upper-left fixed to 0 */
        for (int b = 0; b < 16; ++b) {
            if (!top_ok[a][b]) continue;
            dp[a][b] += 1;
        }
    }

    /* iterate remaining rows (positions 2 … n-1) */
    for (idx_t r = 2; r < n; ++r) {
        /* zero ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                int pair = (prev << 4) | curr;
                int cnt  = next_mid_count[pair];
                for (int k = 0; k < cnt; ++k) {
                    int next = next_mid_rows[pair][k];
                    ndp[curr][next] += ways;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row – no lower neighbours */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (!bot_ok[prev][curr]) continue;
            ans += dp[prev][curr];
        }
    }
    return ans;
}
