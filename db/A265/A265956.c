/*
 * A265956  Number of nX2 integer arrays with each element equal to the number
 *          of horizontal, vertical, diagonal and antidiagonal neighbors less
 *          than itself.
 * https://oeis.org/A265956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A265956
   Number of n×2 integer arrays with each element equal to the number of
   horizontal, vertical, diagonal and antidiagonal neighbors less than
   itself.
   Computation is performed by dynamic programming over rows. */

#include <stdbool.h>

/* Number of possible rows: each of the 2 cells can take values 0..5. */
#define NUM_ROWS 36

/* Helper: validity of a cell in the middle row */
static int
valid(int prevIdx, int currIdx, int nextIdx)
{
    /* Decode cell values */
    int cur0  =  currIdx / 6;
    int cur1  =  currIdx % 6;
    int prev0 = (prevIdx >= 0) ?  prevIdx / 6 : 0;
    int prev1 = (prevIdx >= 0) ?  prevIdx % 6 : 0;
    int next0 = (nextIdx >= 0) ?  nextIdx / 6 : 0;
    int next1 = (nextIdx >= 0) ?  nextIdx % 6 : 0;

    for (int col = 0; col < 2; ++col) {
        int val = (col == 0) ? cur0 : cur1;
        int cnt = 0;

        /* horizontal neighbor */
        int horiz = (col == 0) ? cur1 : cur0;
        if (horiz < val) ++cnt;

        /* vertical and diagonal neighbors from previous row */
        if (prevIdx >= 0) {
            int pv_same   = (col == 0) ? prev0 : prev1;
            int pv_other  = (col == 0) ? prev1 : prev0;
            if (pv_same  < val) ++cnt;
            if (pv_other < val) ++cnt;
        }

        /* vertical and diagonal neighbors from next row */
        if (nextIdx >= 0) {
            int nv_same   = (col == 0) ? next0 : next1;
            int nv_other  = (col == 0) ? next1 : next0;
            if (nv_same  < val) ++cnt;
            if (nv_other < val) ++cnt;
        }

        if (cnt != val) return 0;
    }
    return 1;
}

/* Tables for fast lookup; initialized once per process */
static bool   initialized = false;
static uint8_t baseTable[NUM_ROWS][NUM_ROWS];    /* prev = -1  */
static uint8_t lastTable[NUM_ROWS][NUM_ROWS];    /* next = -1  */
static uint8_t validTable[NUM_ROWS][NUM_ROWS][NUM_ROWS];

static void
init_tables(void)
{
    if (initialized) return;

    for (int p = 0; p < NUM_ROWS; ++p)
        for (int c = 0; c < NUM_ROWS; ++c)
            for (int n = 0; n < NUM_ROWS; ++n)
                validTable[p][c][n] = (uint8_t)valid(p, c, n);

    for (int c = 0; c < NUM_ROWS; ++c)
        for (int n = 0; n < NUM_ROWS; ++n)
            baseTable[c][n] = (uint8_t)valid(-1, c, n);

    for (int p = 0; p < NUM_ROWS; ++p)
        for (int c = 0; c < NUM_ROWS; ++c)
            lastTable[p][c] = (uint8_t)valid(p, c, -1);

    initialized = true;
}

seq_t a(idx_t n)
{
    init_tables();

    /* Special case for n = 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < NUM_ROWS; ++r)
            if (valid(-1, r, -1))
                ans += (seq_t)1;
        return ans;
    }

    /* DP arrays: dp_cur[prev][curr] and dp_next[prev][curr] */
    seq_t dp_cur[NUM_ROWS][NUM_ROWS];
    seq_t dp_next[NUM_ROWS][NUM_ROWS];

    /* Initialize dp_cur for the first two rows */
    for (int prev = 0; prev < NUM_ROWS; ++prev)
        for (int curr = 0; curr < NUM_ROWS; ++curr)
            dp_cur[prev][curr] = baseTable[prev][curr] ? (seq_t)1 : (seq_t)0;

    /* Transition for rows 3 … n */
    if (n >= 3) {
        for (int step = 1; step <= (int)(n - 2); ++step) {
            /* Zero dp_next */
            for (int a = 0; a < NUM_ROWS; ++a)
                for (int b = 0; b < NUM_ROWS; ++b)
                    dp_next[a][b] = (seq_t)0;

            /* Transition */
            for (int p = 0; p < NUM_ROWS; ++p) {
                for (int c = 0; c < NUM_ROWS; ++c) {
                    seq_t curWays = dp_cur[p][c];
                    if (curWays == (seq_t)0) continue;
                    for (int nrow = 0; nrow < NUM_ROWS; ++nrow) {
                        if (validTable[p][c][nrow])
                            dp_next[c][nrow] = dp_next[c][nrow] + curWays;
                    }
                }
            }

            /* Swap dp_cur and dp_next */
            for (int i = 0; i < NUM_ROWS; ++i)
                for (int j = 0; j < NUM_ROWS; ++j)
                    dp_cur[i][j] = dp_next[i][j];
        }
    }

    /* Sum over final states satisfying last row constraints */
    seq_t ans = 0;
    for (int p = 0; p < NUM_ROWS; ++p)
        for (int c = 0; c < NUM_ROWS; ++c)
            if (lastTable[p][c])
                ans = ans + dp_cur[p][c];

    return ans;
}
