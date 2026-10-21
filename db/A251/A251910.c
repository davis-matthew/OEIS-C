/*
 * A251910  Number of (n+2)X(1+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 2 or 7.
 * https://oeis.org/A251910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 arrays over {0,1,2,3} with no 3×3 *
 *            sub‑block row, column, diagonal or antidiagonal summing to 2 or
 *            7
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming over pairs of last two rows. */

#include <stddef.h>
#include <stdlib.h>

/* Constants for full set of rows (0..3)³. */
#define TOTAL_CELLS 64          /* 4³ */
#define MAX_VALID_ROWS 52       /* number of rows with sum ≠ 2,7 */
#define MAX_NEIGHBORS MAX_VALID_ROWS   /* maximum outgoing neighbours in the state graph */

/* Global tables, built once. */
static int rowVals[MAX_VALID_ROWS][3];      /* encoded valid rows */
static int rowIndex[TOTAL_CELLS];           /* map from 0..63 to 0..51 or -1 */
static int validCount = 0;

static int neighborIdx[2704][MAX_NEIGHBORS]; /* indices of neighbour r for each state */
static int neighborCount[2704];
static int stateQ[2704];                    /* second component of the state (p,q) */
static int init_done = 0;

/* ------------------------------------------------------------------ */
/* Build list of allowed rows and adjacency for the state graph.       */
static void init(void)
{
    if (init_done) return;
    init_done = 1;

    /* Find rows whose sum is not 2 or 7. */
    for (int code = 0; code < TOTAL_CELLS; ++code) {
        int a = code / 16;
        int b = (code / 4) % 4;
        int c = code % 4;
        int sum = a + b + c;
        if (sum == 2 || sum == 7) {
            rowIndex[code] = -1;
            continue;
        }
        rowIndex[code] = validCount;
        rowVals[validCount][0] = a;
        rowVals[validCount][1] = b;
        rowVals[validCount][2] = c;
        ++validCount;
    }

    /* Build adjacency for all pairs (p,q) → (q,r). */
    int stateID = 0;
    for (int p = 0; p < validCount; ++p) {
        for (int q = 0; q < validCount; ++q, ++stateID) {
            stateQ[stateID] = q;
            int cnt = 0;
            for (int r = 0; r < validCount; ++r) {
                int sumCol0 = rowVals[p][0] + rowVals[q][0] + rowVals[r][0];
                if (sumCol0 == 2 || sumCol0 == 7) continue;
                int sumCol1 = rowVals[p][1] + rowVals[q][1] + rowVals[r][1];
                if (sumCol1 == 2 || sumCol1 == 7) continue;
                int sumCol2 = rowVals[p][2] + rowVals[q][2] + rowVals[r][2];
                if (sumCol2 == 2 || sumCol2 == 7) continue;
                int diag1 = rowVals[p][0] + rowVals[q][1] + rowVals[r][2];
                if (diag1 == 2 || diag1 == 7) continue;
                int diag2 = rowVals[p][2] + rowVals[q][1] + rowVals[r][0];
                if (diag2 == 2 || diag2 == 7) continue;
                neighborIdx[stateID][cnt++] = r;
            }
            neighborCount[stateID] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init();

    const size_t V = (size_t)validCount;        /* 52 */
    const size_t STATES = V * V;                /* 2704 */
    const idx_t steps = n;                     /* extend from the first two rows */

    /* Dynamic arrays for DP (allocated once). */
    static seq_t *cur = NULL;
    static seq_t *nxt = NULL;
    static size_t allocated = 0;

    if (cur == NULL || allocated < STATES) {
        cur  = (seq_t*)realloc(cur,  STATES * sizeof(seq_t));
        nxt  = (seq_t*)realloc(nxt,  STATES * sizeof(seq_t));
        allocated = STATES;
    }

    /* Initialise: one way for each starting pair of rows. */
    for (size_t i = 0; i < STATES; ++i) cur[i] = 1;

    /* Iteratively extend the array one row at a time. */
    for (idx_t step = 0; step < steps; ++step) {
        /* zero destination */
        for (size_t i = 0; i < STATES; ++i) nxt[i] = 0;

        /* transition */
        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = cur[idx];
            if (val == 0) continue;
            int q = stateQ[idx];
            int cnt = neighborCount[idx];
            int *nei = neighborIdx[idx];
            for (int j = 0; j < cnt; ++j) {
                int r = nei[j];
                size_t nextIdx = (size_t)q * V + r;
                nxt[nextIdx] += val;
            }
        }

        /* swap */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all ending states. */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += cur[i];
    return total;
}
