/*
 * A303522  Number of nX5 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303522:
 * a(n) = number of n×5 binary arrays with the upper‑left element fixed 0
 * such that every cell has 0,1,3,4,5 or 6 equal neighbours among its
 * six horizontally, diagonally or antidiagonally adjacent cells.
 * (Neighbour count 2 is forbidden.) */

#include <stdbool.h>

#define ROWS   32          /* 2^5 possible row patterns */
#define SENTIN 32          /* sentinel value for "no row" */

/* tables – initialised once */
static bool  valid[SENTIN+1][ROWS][SENTIN+1];
static uint32_t nextMask[SENTIN+1][ROWS];
static seq_t dpPrev[SENTIN+1][ROWS];
static seq_t dpNext[SENTIN+1][ROWS];
static bool tables_initialized = false;

/* Build the validity tables:  valid[prev][curr][next] is true iff
 * all cells of *curr* satisfy the neighbour rule when *prev* precedes
 * it and *next* follows it.  next may be SENTIN to denote a missing
 * neighbour row (top or bottom edge). */
static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = true;

    for (int prev = 0; prev <= SENTIN; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            for (int next = 0; next <= SENTIN; ++next) {
                bool ok = true;
                for (int c = 0; c < 5 && ok; ++c) {
                    int bitCur = (curr >> c) & 1;
                    int cnt = 0;

                    /* left / right in the same row */
                    if (c > 0   && ((curr >> (c-1)) & 1) == bitCur) ++cnt;
                    if (c < 4   && ((curr >> (c+1)) & 1) == bitCur) ++cnt;

                    /* diagonal neighbours from the previous row */
                    if (prev != SENTIN && c > 0
                            && ((prev >> (c-1)) & 1) == bitCur) ++cnt;
                    if (prev != SENTIN && c < 4
                            && ((prev >> (c+1)) & 1) == bitCur) ++cnt;

                    /* diagonal neighbours from the next row */
                    if (next != SENTIN && c > 0
                            && ((next >> (c-1)) & 1) == bitCur) ++cnt;
                    if (next != SENTIN && c < 4
                            && ((next >> (c+1)) & 1) == bitCur) ++cnt;

                    if (cnt == 2) { ok = false; break; }
                }
                valid[prev][curr][next] = ok;
            }
        }
    }

    /* pre‑compute transition masks */
    for (int prev = 0; prev <= SENTIN; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            uint32_t mask = 0;
            for (int next = 0; next < ROWS; ++next) {
                if (valid[prev][curr][next]) mask |= (1u << next);
            }
            nextMask[prev][curr] = mask;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for non‑offset inputs */
    init_tables();

    /* initialise dpPrev to zero */
    for (int i = 0; i <= SENTIN; ++i)
        for (int j = 0; j < ROWS; ++j)
            dpPrev[i][j] = 0;

    /* first row: prev = SENTIN, curr any row with upper‑left bit 0 */
    for (int row = 0; row < ROWS; ++row)
        if ((row & 1) == 0)             /* upper‑left element is 0 */
            dpPrev[SENTIN][row] = 1;

    /* add rows 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear dpNext */
        for (int i = 0; i <= SENTIN; ++i)
            for (int j = 0; j < ROWS; ++j)
                dpNext[i][j] = 0;

        /* transitions */
        for (int prev = 0; prev <= SENTIN; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dpPrev[prev][curr];
                if (!val) continue;
                for (int nxt = 0; nxt < ROWS; ++nxt) {
                    if (valid[prev][curr][nxt])
                        dpNext[curr][nxt] = dpNext[curr][nxt] + val;
                }
            }
        }

        /* swap dpPrev and dpNext */
        for (int i = 0; i <= SENTIN; ++i)
            for (int j = 0; j < ROWS; ++j) {
                dpPrev[i][j] = dpNext[i][j];
                dpNext[i][j] = 0;      /* reset for next iteration */
            }
    }

    /* final validation – last row must be consistent with no following row */
    seq_t total = 0;
    for (int prev = 0; prev <= SENTIN; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            if (valid[prev][curr][SENTIN])
                total = total + dpPrev[prev][curr];
        }
    }
    return total;
}
