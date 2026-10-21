/*
 * A267899  Number of nX3 arrays containing 3 copies of 0..n-1 with every
 *          element equal to at least one horizontal or vertical neighbor and
 *          the top left element equal to 0.
 * https://oeis.org/A267899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * f(n) where f(n) is the number of tilings of a
 *            3×n rectangle with trominoes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"

typedef struct {
    int curMask;      /* bits of cells in the current column (offset 0) */
    int nextMask;     /* bits of cells in the next column (offset 1) */
    int nextnextMask; /* bits of cells in the next‑next column (offset 2) */
} Shape;

static Shape shapesRow[3][20];
static int  shapesCnt[3] = {0,0,0};
static bool shapesInit = false;

/* ---------------------------------------------------------------------- */
/* Enumerate all tromino shapes that can be placed with the leftmost cell
   in column 0.  For each row we store those shapes that contain a cell
   at row r in the current column. */
static void init_shapes(void)
{
    if (shapesInit) return;
    shapesInit = true;

    int cells[9][2];           /* row, col for each of the 9 cells */
    for (int idx = 0; idx < 9; ++idx) {
        cells[idx][0] = idx / 3;  /* row */
        cells[idx][1] = idx % 3;  /* col (0,1,2) */
    }

    /* Enumerate all triples of cells */
    for (int a = 0; a < 7; ++a) {
        for (int b = a + 1; b < 8; ++b) {
            for (int c = b + 1; c < 9; ++c) {
                int idx[3] = {a, b, c};
                /* connectivity test (BFS) */
                bool vis[9] = {0};
                int q[3], qs = 0, qe = 0;
                q[qe++] = a; vis[a] = true;
                while (qs < qe) {
                    int u = q[qs++];
                    for (int t = 0; t < 3; ++t) {
                        int v = idx[t];
                        if (vis[v]) continue;
                        int dr = abs(cells[u][0] - cells[v][0]);
                        int dc = abs(cells[u][1] - cells[v][1]);
                        if (dr + dc == 1) {
                            vis[v] = true;
                            q[qe++] = v;
                        }
                    }
                }
                if (qe != 3) continue;          /* not connected */

                /* shape must contain at least one cell in column 0 */
                bool has0 = false;
                for (int t = 0; t < 3; ++t)
                    if (cells[idx[t]][1] == 0) { has0 = true; break; }
                if (!has0) continue;

                /* build masks */
                int cur = 0, next = 0, nextnext = 0;
                for (int t = 0; t < 3; ++t) {
                    int row = cells[idx[t]][0];
                    int col = cells[idx[t]][1];
                    if (col == 0) cur      |= (1 << row);
                    else if (col == 1) next |= (1 << row);
                    else                nextnext |= (1 << row);
                }

                /* add to lists for every row contained in curMask */
                for (int r = 0; r < 3; ++r) {
                    if (cur & (1 << r)) {
                        Shape *s = &shapesRow[r][shapesCnt[r]++];
                        s->curMask = cur;
                        s->nextMask = next;
                        s->nextnextMask = nextnext;
                    }
                }
            }
        }
    }
}

/* Recursive fill of a single column, given the current occupancy masks. */
static void dfs_fill(int curMask, int nextMask, int A, int B,
                     int remaining, seq_t cnt, seq_t dp_next[8][8])
{
    if (curMask == 7) {               /* column completely filled */
        int newCur = nextMask | A;    /* occupancy in the next column */
        int newNext = B;              /* occupancy in the next‑next column */
        dp_next[newCur][newNext] += cnt;
        return;
    }

    /* find first empty row in the current column */
    int r = 0;
    while (curMask & (1 << r)) r++;

    /* try all shapes that contain (r, offset 0) */
    for (int i = 0; i < shapesCnt[r]; ++i) {
        const Shape *sh = &shapesRow[r][i];

        /* respect board boundaries */
        if (remaining == 1 && (sh->nextMask || sh->nextnextMask)) continue;
        if (remaining == 2 && sh->nextnextMask) continue;

        /* check for overlap with already occupied cells */
        if ( (curMask & sh->curMask) != 0 ) continue;
        int usedNext = nextMask | A;
        if ( (usedNext & sh->nextMask) != 0 ) continue;
        if ( (B & sh->nextnextMask) != 0 ) continue;

        int newCur = curMask | sh->curMask;
        int newA   = A     | sh->nextMask;
        int newB   = B     | sh->nextnextMask;
        dfs_fill(newCur, nextMask, newA, newB, remaining, cnt, dp_next);
    }
}

/* ---------------------------------------------------------------------- */
/* Main function computing A267899. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not needed for the sequence */

    init_shapes();

    static seq_t dp[2][8][8] = {0};
    static seq_t ndp[8][8] = {0};
    int curIdx = 0, nxtIdx;

    /* initialize dp */
    for (int i=0;i<8;++i)for(int j=0;j<8;++j)dp[curIdx][i][j] = 0;
    dp[curIdx][0][0] = 1;               /* start with empty board */

    for (idx_t col = 0; col < n; ++col) {
        nxtIdx = 1 - curIdx;
        /* clear next dp */
        for (int i=0;i<8;++i)for(int j=0;j<8;++j)ndp[i][j] = 0;

        int remaining = n - col;        /* columns left including current */

        for (int curMask = 0; curMask < 8; ++curMask) {
            for (int nextMask = 0; nextMask < 8; ++nextMask) {
                seq_t cnt = dp[curIdx][curMask][nextMask];
                if (cnt == 0) continue;
                dfs_fill(curMask, nextMask, 0, 0, remaining, cnt, ndp);
            }
        }

        /* swap dp arrays */
        for (int i=0;i<8;++i)for(int j=0;j<8;++j)dp[nxtIdx][i][j] = ndp[i][j];
        curIdx = nxtIdx;
    }

    seq_t tilings = dp[curIdx][0][0];
    seq_t fact    = factorial(n - 1);  /* (n-1)!  */
    return tilings * fact;
}
