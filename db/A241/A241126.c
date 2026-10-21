/*
 * A241126  Number of nX3 0..2 arrays with no element equal to exactly two
 *          horizontal and vertical neighbors, with new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A241126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×3 0..2 arrays with no element equal to exactly
   two horizontal and vertical neighbors, with new values 0..2 introduced
   in row major order.  The computation uses a transfer‑matrix DP over
   rows and a single bit flag that records whether value 1 has already
   appeared somewhere before the current row.  */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Constants for the number of possible row patterns (3^3=27). */
#define ROWS 27
#define NONE 27  /* sentinel for “no row” */

static bool init_table = false;
static uint8_t row_digits[ROWS][3];
static bool  has1[ROWS];
static bool  has2[ROWS];
static bool  valid_startflag[2][ROWS];
static bool  valid_neigh[ROWS+1][ROWS+1][ROWS+1];

static void init_tables(void)
{
    if (init_table) return;
    init_table = true;

    /* Enumerate all row patterns and pre‑compute helpers. */
    for (int r = 0; r < ROWS; ++r) {
        int t = r;
        for (int c = 2; c >= 0; --c) {
            row_digits[r][c] = t % 3;
            t /= 3;
        }
        has1[r] = false;
        has2[r] = false;
        for (int c = 0; c < 3; ++c) {
            if (row_digits[r][c] == 1) has1[r] = true;
            if (row_digits[r][c] == 2) has2[r] = true;
        }
    }

    /* Compute prefix‑rule validity for each row.  The flag indicates
       whether value 1 has already appeared before this row (0 = no,
       1 = yes).  */
    for (int flag = 0; flag < 2; ++flag) {
        for (int r = 0; r < ROWS; ++r) {
            if (flag == 1) {
                valid_startflag[flag][r] = true;
                continue;
            }
            /* flag == 0 : 1 must occur before any 2. */
            int first1 = 3, first2 = 3;
            for (int c = 0; c < 3; ++c) {
                if (row_digits[r][c] == 1) {
                    if (first1 == 3) first1 = c;
                }
                if (row_digits[r][c] == 2) {
                    if (first2 == 3) first2 = c;
                }
            }
            if (has2[r] && first2 < first1)  /* 2 appears before first 1 */
                valid_startflag[flag][r] = false;
            else
                valid_startflag[flag][r] = true;
        }
    }

    /* Pre‑compute neighbor validity of triples (up,mid,down). */
    for (int up = 0; up <= ROWS; ++up) {
        for (int mid = 0; mid <= ROWS; ++mid) {
            for (int down = 0; down <= ROWS; ++down) {
                bool ok = true;
                for (int c = 0; c < 3 && ok; ++c) {
                    int cnt = 0;
                    /* left */
                    if (c > 0 && row_digits[mid][c-1] == row_digits[mid][c]) ++cnt;
                    /* right */
                    if (c < 2 && row_digits[mid][c+1] == row_digits[mid][c]) ++cnt;
                    /* up */
                    if (up != NONE && row_digits[up][c] == row_digits[mid][c]) ++cnt;
                    /* down */
                    if (down != NONE && row_digits[down][c] == row_digits[mid][c]) ++cnt;
                    if (cnt == 2) ok = false;
                }
                valid_neigh[up][mid][down] = ok;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays: dp[up][mid][flag] where flag is 0 (no 1 yet) or 1 (1 seen). */
    static seq_t dp[ROWS+1][ROWS+1][2];
    static seq_t ndp[ROWS+1][ROWS+1][2];
    memset(dp, 0, sizeof(dp));

    /* Initial state: only the first row has been placed. */
    for (int r = 0; r < ROWS; ++r) {
        if (row_digits[r][0] != 0) continue;                 /* first cell must be 0 */
        if (!valid_startflag[0][r])   continue;              /* prefix rule with no 1 seen */
        dp[NONE][r][0] = 1;                                 /* flag before first row = 0 */
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int up = 0; up <= ROWS; ++up) {
            for (int mid = 0; mid <= ROWS; ++mid) {
                for (int f = 0; f < 2; ++f) {
                    seq_t ways = dp[up][mid][f];
                    if (!ways) continue;
                    int flag_before_cur = f | (has1[mid] ? 1 : 0);
                    for (int cur = 0; cur < ROWS; ++cur) {
                        if (!valid_startflag[flag_before_cur][cur]) continue;
                        if (!valid_neigh[up][mid][cur])        continue;
                        ndp[mid][cur][flag_before_cur] += ways;
                    }
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final validation of the last row */
    seq_t ans = 0;
    for (int up = 0; up <= ROWS; ++up) {
        for (int mid = 0; mid <= ROWS; ++mid) {
            for (int f = 0; f < 2; ++f) {
                seq_t ways = dp[up][mid][f];
                if (!ways) continue;
                if (!valid_neigh[up][mid][NONE]) continue;  /* no down neighbor */
                ans += ways;
            }
        }
    }
    return ans;
}
