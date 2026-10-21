/*
 * A252598  Number of (n+2)X(1+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 2 or 5.
 * https://oeis.org/A252598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 arrays over {0,1,2} such that * every
 *            3×3 sub‑block has no row, column, diagonal or * antidiagonal sum
 *            equal to 1, 2 or 5. The implementation uses * dynamic
 *            programming over pairs of consecutive rows
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 43, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define COUNT 27                          /* 3³ possible rows of 3 digits (0–2) */

static int     rowDig[COUNT][3];          /* digits of each row */
static int     rowSum[COUNT];             /* sum of the digits of each row */
static int     to[COUNT*COUNT][COUNT];    /* adjacency list indices */
static uint8_t toCnt[COUNT*COUNT];        /* number of neighbours for each state */
static bool    ready = false;             /* initialisation flag */

/* Initialise lookup tables once */
static void init_tables(void)
{
    if (ready) return;

    /* Pre‑compute row digits and row sums */
    for (int r = 0; r < COUNT; ++r) {
        int t = r;
        for (int j = 0; j < 3; ++j) {
            rowDig[r][j] = t % 3;
            t /= 3;
        }
        rowSum[r] = rowDig[r][0] + rowDig[r][1] + rowDig[r][2];
    }

    /* Pre‑compute transitions: from every pair (a,b) to every possible
     * third row c that satisfies all conditions for the block a,b,c. */
    for (int a = 0; a < COUNT; ++a) {
        for (int b = 0; b < COUNT; ++b) {
            int idx = a * COUNT + b;
            int cnt = 0;
            for (int c = 0; c < COUNT; ++c) {
                /* Row sums not allowed to be 1, 2 or 5 */
                if (rowSum[a] == 1 || rowSum[a] == 2 || rowSum[a] == 5) continue;
                if (rowSum[b] == 1 || rowSum[b] == 2 || rowSum[b] == 5) continue;
                if (rowSum[c] == 1 || rowSum[c] == 2 || rowSum[c] == 5) continue;

                /* Column sums not allowed to be 1, 2 or 5 */
                bool bad = false;
                for (int col = 0; col < 3; ++col) {
                    int s = rowDig[a][col] + rowDig[b][col] + rowDig[c][col];
                    if (s == 1 || s == 2 || s == 5) { bad = true; break; }
                }
                if (bad) continue;

                /* Main diagonal */
                int diag1 = rowDig[a][0] + rowDig[b][1] + rowDig[c][2];
                if (diag1 == 1 || diag1 == 2 || diag1 == 5) continue;

                /* Antidiagonal */
                int diag2 = rowDig[a][2] + rowDig[b][1] + rowDig[c][0];
                if (diag2 == 1 || diag2 == 2 || diag2 == 5) continue;

                to[idx][cnt++] = c;
            }
            toCnt[idx] = (uint8_t)cnt;
        }
    }

    ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    init_tables();

    const int ST = COUNT * COUNT;   /* 729 states: pair of last two rows */
    seq_t cur[ST];
    seq_t nxt[ST];

    /* All possible pairs of the first two rows are initially allowed */
    for (int i = 0; i < ST; ++i) cur[i] = 1;

    /* Add n rows: total height becomes (n+2) */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ST; ++i) nxt[i] = 0;
        for (int idx = 0; idx < ST; ++idx) {
            seq_t val = cur[idx];
            if (val == 0) continue;
            int a = idx / COUNT;
            int b = idx % COUNT;
            int cnt = toCnt[idx];
            int *ptr = to[idx];
            for (int k = 0; k < cnt; ++k) {
                int c = ptr[k];
                int nidx = b * COUNT + c;
                nxt[nidx] += val;
            }
        }
        /* Prepare for next iteration */
        for (int i = 0; i < ST; ++i) cur[i] = nxt[i];
    }

    /* Sum all possible ending pairs */
    seq_t total = 0;
    for (int i = 0; i < ST; ++i) total += cur[i];
    return total;
}
