/*
 * A298089  Number of nX4 0..1 arrays with every element equal to 1, 2, 4 or 7
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming computation of OEIS A298089 
   (n × 4 binary arrays, upper‑left 0, each cell has 1, 2, 4 or 7 equal
    king‑move neighbours).  No closed formula is known; the algorithm
    explores row by row and counts all admissible configurations.   */

#include <stdint.h>
#include <stdbool.h>

/* 4 columns per row */
#define WIDTH 4
/* Masks 0 … 15 represent a 4‑bit row; optional value 16 = "no row" */
#define NONE 16
#define NUM_ROWS 17   /* 15 masks + NONE */
#define NUM_MASKS 16  /* 0‑15 */

/* adjacency counts allowed for a cell */
static const unsigned char allowed[9] = {
    0, 1, 1, 0, 1, 0, 0, 1, 0
};

/* check whether the middle row (maskB) satisfies the adjacency rule
   given the row above (maskA, existsA) and row below (maskC, existsC). */
static int check_row(uint8_t maskA, bool existsA,
                     uint8_t maskB,
                     uint8_t maskC, bool existsC)
{
    for (int col = 0; col < WIDTH; ++col) {
        int total = 0;
        int ones  = 0;

        for (int dr = -1; dr <= 1; ++dr) {
            bool exists = (dr == -1) ? existsA :
                          (dr == 0)  ? true   :
                                      existsC;
            if (!exists) continue;

            uint8_t rowMask = (dr == -1) ? maskA :
                              (dr == 0)  ? maskB :
                                          maskC;

            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;      /* skip the cell itself */
                int nc = col + dc;
                if (nc < 0 || nc >= WIDTH) continue;   /* outside board */
                ++total;
                if ((rowMask >> nc) & 1u) ++ones;
            }
        }

        int bit   = (maskB >> col) & 1u;
        int same  = bit ? ones : (total - ones);
        if (!allowed[same]) return 0;
    }
    return 1;
}

/* public entry point for the sequence */
seq_t a(idx_t n)
{
    /* size of last row count table */
    static seq_t dp[NUM_ROWS][NUM_MASKS];
    static seq_t next[NUM_ROWS][NUM_MASKS];
    int i, j, cur;

    /* handle n == 1 separately */
    if (n == 1) {
        seq_t result = 0;
        for (cur = 0; cur < NUM_MASKS; ++cur) {
            if (cur & 1u) continue;               /* upper‑left must be 0 */
            if (check_row(0, false, (uint8_t)cur,
                          0, false))
                result += 1;
        }
        return result;
    }

    /* initialise dp table to zero */
    for (i = 0; i < NUM_ROWS; ++i)
        for (j = 0; j < NUM_MASKS; ++j)
            dp[i][j] = 0;

    /* first row: maskA is non‑existent, so existsA = false */
    for (cur = 0; cur < NUM_MASKS; ++cur) {
        if (cur & 1u) continue;                    /* upper‑left must be 0 */
        dp[NONE][cur] = dp[NONE][cur] + 1;
    }

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next table */
        for (i = 0; i < NUM_ROWS; ++i)
            for (j = 0; j < NUM_MASKS; ++j)
                next[i][j] = 0;

        /* transition from all (prevRow, curRow) states */
        for (int prevRow = 0; prevRow < NUM_ROWS; ++prevRow) {
            bool existsA = (prevRow != NONE);
            for (int curRow = 0; curRow < NUM_MASKS; ++curRow) {
                seq_t val = dp[prevRow][curRow];
                if (val == 0) continue;
                for (cur = 0; cur < NUM_MASKS; ++cur) {
                    if (check_row(prevRow == NONE ? 0 : (uint8_t)prevRow,
                                  existsA,
                                  (uint8_t)curRow,
                                  (uint8_t)cur,
                                  true))   /* new row always exists */
                        next[curRow][cur] = next[curRow][cur] + val;
                }
            }
        }

        /* copy next into dp for next iteration */
        for (i = 0; i < NUM_ROWS; ++i)
            for (j = 0; j < NUM_MASKS; ++j)
                dp[i][j] = next[i][j];
    }

    /* final evaluation: check last row constraints with non‑existent below */
    seq_t result = 0;
    for (int prevRow = 0; prevRow < NUM_ROWS; ++prevRow) {
        for (int curRow = 0; curRow < NUM_MASKS; ++curRow) {
            seq_t val = dp[prevRow][curRow];
            if (val == 0) continue;
            if (check_row((uint8_t)prevRow,
                          true,            /* previous row exists */
                          (uint8_t)curRow,
                          0, false))       /* below row does not exist */
                result = result + val;
        }
    }
    return result;
}
