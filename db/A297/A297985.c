/*
 * A297985  Number of nX7 0..1 arrays with every element equal to 0, 2, 3 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A297985: Number of n×7 binary arrays with
    every element having 0, 2, 3 or 6 equal
    king-move neighbours and a(1) the first term.
    Computed by a transfer‑matrix DP over successive rows.
    The algorithm handles the boundary rows correctly
    by treating a missing neighbour as “non‑existent”.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define WIDTH          7
#define STATE_DIM      (1 << WIDTH)    /* 128 possible rows */
#define TOTAL_STATES   (STATE_DIM + 1) /* plus one sentinel for “no row” */
#define NONE           STATE_DIM       /* value 128 used as sentinel */

static uint64_t nextMaskLow[TOTAL_STATES][STATE_DIM];
static uint64_t nextMaskHigh[TOTAL_STATES][STATE_DIM];
static bool     masksReady = false;

/* --------------------------------------------------------------- */
/*  Test whether “curr” can be placed between “prev” and “next”.
    If prev or next equals NONE, the corresponding neighbours are omitted.
    Only cells in column 0…6 are considered, with upper‑left zero already
    enforced externally.
*/
static inline bool valid_row(int prev, int curr, int next)
{
    for (int j = 0; j < WIDTH; ++j) {
        int cur_bit = (curr >> j) & 1;
        int count   = 0;

        /* neighbours from the row above */
        if (prev != NONE) {
            for (int k = j - 1; k <= j + 1; ++k) {
                if (k < 0 || k >= WIDTH) continue;
                int nb = (prev >> k) & 1;
                if (nb == cur_bit) ++count;
            }
        }

        /* neighbours in the same row (horizontal) */
        for (int k = j - 1; k <= j + 1; ++k) {
            if (k < 0 || k >= WIDTH || k == j) continue;
            int nb = (curr >> k) & 1;
            if (nb == cur_bit) ++count;
        }

        /* neighbours from the row below */
        if (next != NONE) {
            for (int k = j - 1; k <= j + 1; ++k) {
                if (k < 0 || k >= WIDTH) continue;
                int nb = (next >> k) & 1;
                if (nb == cur_bit) ++count;
            }
        }

        /* allowed counts are 0, 2, 3 or 6 */
        if (!(count == 0 || count == 2 || count == 3 || count == 6))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/*  Pre‑compute for every (prev, curr) pair the set of next rows
    that keep the middle row valid.  The table uses 64‑bit masks
    split into low and high parts because 128 rows cannot be
    expressed with a single 64‑bit mask.
*/
static void build_masks(void)
{
    if (masksReady) return;
    for (int p = 0; p < TOTAL_STATES; ++p) {
        for (int c = 0; c < STATE_DIM; ++c) {
            uint64_t low = 0, high = 0;
            for (int n = 0; n < STATE_DIM; ++n) {
                if (valid_row(p, c, n)) {
                    if (n < 64)
                        low |= 1ULL << n;
                    else
                        high |= 1ULL << (n - 64);
                }
            }
            nextMaskLow[p][c]  = low;
            nextMaskHigh[p][c] = high;
        }
    }
    masksReady = true;
}

/* --------------------------------------------------------------- */
/*  Main function: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below the offset – harmless */

    build_masks();

    static seq_t dp[2][TOTAL_STATES][STATE_DIM];
    int cur = 0, nxt = 1;

    /* initialise all counts to zero */
    for (int t = 0; t < 2; ++t)
        for (int p = 0; p < TOTAL_STATES; ++p)
            for (int c = 0; c < STATE_DIM; ++c)
                dp[t][p][c] = (seq_t)0;

    /* first row: upper‑left must be 0 (least significant bit). */
    for (int row = 0; row < STATE_DIM; ++row) {
        if (row & 1) continue;                 /* column 0 must be 0 */
        dp[cur][NONE][row] = (seq_t)1;          /* no row above */
    }

    /* iterate over the remaining rows */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear the next layer */
        for (int p = 0; p < TOTAL_STATES; ++p)
            for (int c = 0; c < STATE_DIM; ++c)
                dp[nxt][p][c] = (seq_t)0;

        /* transition from dp[cur] to dp[nxt] */
        for (int prev = 0; prev < TOTAL_STATES; ++prev) {
            for (int curr = 0; curr < STATE_DIM; ++curr) {
                seq_t val = dp[cur][prev][curr];
                if (val == (seq_t)0) continue;
                uint64_t maskL = nextMaskLow[prev][curr];
                while (maskL) {
                    int nrow = __builtin_ctzll(maskL);
                    dp[nxt][curr][nrow] += val;
                    maskL &= maskL - 1;
                }
                uint64_t maskH = nextMaskHigh[prev][curr];
                while (maskH) {
                    int nrow = __builtin_ctzll(maskH) + 64;
                    dp[nxt][curr][nrow] += val;
                    maskH &= maskH - 1;
                }
            }
        }
        /* prepare for the next iteration */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final validation of the last row (no below neighbours) */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < TOTAL_STATES; ++prev) {
        for (int curr = 0; curr < STATE_DIM; ++curr) {
            seq_t val = dp[cur][prev][curr];
            if (val == (seq_t)0) continue;
            if (valid_row(prev, curr, NONE))
                result += val;
        }
    }

    return result;
}
