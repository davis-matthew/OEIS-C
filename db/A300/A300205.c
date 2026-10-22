/*
 * A300205  Number of n X 5 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 4, 5, 6 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A300205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A300205: number of n×5 binary arrays with upper‑left
 * fixed to 0 and no interior cell having exactly 7 neighbouring cells
 * (king moves) with the same value.  The constraint is checked row by
 * row during a dynamic programming traversal. */

#include <stddef.h>
#include <stdbool.h>

/* Constants for the DP */
#define WIDTH 5
#define ROWS 33           /* 32 patterns (0–31) + 1 sentinel */
#define SENTINEL 32

/* Forward declaration of helpers supplied by the build */
extern idx_t _floor_isqrt_u64(idx_t n);

/* Transition tables: for each pair (prev, cur) we store the list of
 * next rows that keep cur valid with respect to prev and that next row. */
static unsigned char nextCnt[ROWS][ROWS];
static unsigned char nextRows[ROWS][ROWS][ROWS];
static int precomputed = 0;

/* --------------------------------------------------------------------- */
/*  Row validity test.  For a given row pattern, we are given the
 *  patterns of the row above and the row below (SENTINEL means no
 *  such row).  The function returns true if the whole row satisfies
 *  the “no interior cell has exactly 7 equal neighbours” rule.      */
static bool row_valid(unsigned int row, unsigned int above,
                      unsigned int below)
{
    for (int j = 0; j < WIDTH; ++j) {
        unsigned int val = (row >> j) & 1u;
        unsigned int cnt = 0;

        /* Same‑row neighbours */
        if (j > 0)
            if (((row >> (j - 1)) & 1u) == val) ++cnt;
        if (j + 1 < WIDTH)
            if (((row >> (j + 1)) & 1u) == val) ++cnt;

        /* Upper‑row neighbours */
        if (above != SENTINEL) {
            if (((above >> j) & 1u) == val) ++cnt;
            if (j > 0)
                if (((above >> (j - 1)) & 1u) == val) ++cnt;
            if (j + 1 < WIDTH)
                if (((above >> (j + 1)) & 1u) == val) ++cnt;
        }

        /* Lower‑row neighbours */
        if (below != SENTINEL) {
            if (((below >> j) & 1u) == val) ++cnt;
            if (j > 0)
                if (((below >> (j - 1)) & 1u) == val) ++cnt;
            if (j + 1 < WIDTH)
                if (((below >> (j + 1)) & 1u) == val) ++cnt;
        }

        /* Interior cells have both above and below rows present.
         * Only such cells are subject to the forbidden count of 7. */
        if (above != SENTINEL && below != SENTINEL &&
            j > 0 && j + 1 < WIDTH && cnt == 7u)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Build transition tables once.  For every pair (a,b) (a is the
 *  pattern of the row above, b the current pattern) compute all
 *  possible patterns c that can follow.  The tables are indexed
 *  with the sentinel value for a.  Only actual rows 0‑31 are used for
 *  the current and next rows. */
static void init_transitions(void)
{
    if (precomputed) return;
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS - 1; ++b) {   /* b cannot be sentinel */
            unsigned char *list = nextRows[a][b];
            unsigned char cnt = 0;
            for (int c = 0; c < ROWS - 1; ++c) {
                if (row_valid(b, a, c))
                    list[cnt++] = c;
            }
            nextCnt[a][b] = cnt;
        }
    }
    precomputed = 1;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.  The DP arrays are indexed by (prev,cur)
 *  where prev is the row above cur (0‑31 for real rows, 32 sentinel)
 *  and cur is the current row (0‑31).  The DP counts the number of
 *  ways to fill rows 0..i with the given state. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    init_transitions();

    seq_t dp[ROWS][ROWS] = {0};
    seq_t next[ROWS][ROWS] = {0};

    /* Initialise with the first row.  The upper‑left element must be 0. */
    for (int r = 0; r < ROWS - 1; ++r) {
        if ((r & 1u) == 0)   /* bit 0 (column 0) is zero */
            dp[SENTINEL][r] = 1;
    }

    /* Process the remaining rows. */
    for (idx_t i = 1; i < n; ++i) {
        /* Clear next array. */
        for (int x = 0; x < ROWS; ++x)
            for (int y = 0; y < ROWS; ++y)
                next[x][y] = (seq_t)0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS - 1; ++cur) {
                seq_t curcnt = dp[prev][cur];
                if (curcnt == (seq_t)0) continue;
                unsigned char nc = nextCnt[prev][cur];
                unsigned char *list = nextRows[prev][cur];
                for (unsigned char k = 0; k < nc; ++k) {
                    int nxt = list[k];
                    next[cur][nxt] += curcnt;
                }
            }
        }

        /* Copy next to dp for the next iteration. */
        for (int x = 0; x < ROWS; ++x)
            for (int y = 0; y < ROWS; ++y)
                dp[x][y] = next[x][y];
    }

    /* Final validation: the last row has no row below. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS - 1; ++cur) {
            seq_t curcnt = dp[prev][cur];
            if (curcnt == (seq_t)0) continue;
            if (row_valid(cur, prev, SENTINEL))
                ans += curcnt;
        }
    }

    return ans;
}
