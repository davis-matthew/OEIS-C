/*
 * A252287  Number of (n+2)X(2+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 1 or 4.
 * https://oeis.org/A252287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 arrays over {0,1,2} with every 3×3
 *            subblock row, column, diagonal and antidiagonal sums not equal
 *            to 0, 1 or 4. Computed by dynamic programming over the rows
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 38, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Number of possible row patterns: 3^4 */
#define NROWS 81
/* Number of states for a pair of consecutive rows */
#define NP (NROWS * NROWS)

/* Precomputed data structures */
static uint8_t  g_rows[NROWS][4];          /* value at each column for each pattern */
static uint8_t  g_edges[NP][NROWS];        /* list of admissible next rows for each pair */
static uint8_t  g_edges_len[NP];           /* number of admissible next rows */
static int      g_initialized = 0;

/* --------------------------------------------------------------------- */
/* Helper that builds all tables on the first call. */
static void prepare_tables(void)
{
    /* 1. Encode every row pattern */
    for (int pat = 0; pat < NROWS; ++pat) {
        int v = pat;
        for (int col = 0; col < 4; ++col) {
            g_rows[pat][col] = v % 3;
            v /= 3;
        }
    }

    /* 2. For every ordered pair (prevprev, prev) determine
       which next rows are allowed. */
    for (int a = 0; a < NROWS; ++a) {
        for (int b = 0; b < NROWS; ++b) {
            size_t idx = (size_t)a * NROWS + b;
            uint8_t len = 0;

            for (int c = 0; c < NROWS; ++c) {
                int ok = 1;

                /* Check both 3×3 windows (starting at column 0 and 1) */
                for (int j = 0; j <= 1 && ok; ++j) {
                    /* Row sums of the 3 rows */
                    for (int r = 0; r < 3; ++r) {
                        int rowIdx = (r == 0) ? a : ((r == 1) ? b : c);
                        int s = g_rows[rowIdx][j] + g_rows[rowIdx][j + 1] + g_rows[rowIdx][j + 2];
                        if (s == 0 || s == 1 || s == 4) { ok = 0; break; }
                    }

                    if (!ok) break;

                    /* Column sums of the 3 columns */
                    for (int col = j; col <= j + 2; ++col) {
                        int s = g_rows[a][col] + g_rows[b][col] + g_rows[c][col];
                        if (s == 0 || s == 1 || s == 4) { ok = 0; break; }
                    }

                    if (!ok) break;

                    /* Main diagonal */
                    int s = g_rows[a][j] + g_rows[b][j + 1] + g_rows[c][j + 2];
                    if (s == 0 || s == 1 || s == 4) ok = 0;
                    if (!ok) break;

                    /* Anti‑diagonal */
                    s = g_rows[a][j + 2] + g_rows[b][j + 1] + g_rows[c][j];
                    if (s == 0 || s == 1 || s == 4) ok = 0;
                }

                if (ok) {
                    g_edges[idx][len++] = (uint8_t)c;
                }
            }

            g_edges_len[idx] = len;
        }
    }

    g_initialized = 1;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (!g_initialized) prepare_tables();

    /* DP tables: counts for each pair (prevprev, prev) */
    static seq_t dp_cur[NP];
    static seq_t dp_next[NP];

    /* Initial state: after two rows, all pairs are possible, count = 1 */
    for (size_t i = 0; i < NP; ++i) dp_cur[i] = (seq_t)1;

    /* Number of additional rows to add = (n+2) – 2 = n */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next array */
        for (size_t i = 0; i < NP; ++i) dp_next[i] = (seq_t)0;

        /* Transition */
        for (size_t p = 0; p < NP; ++p) {
            seq_t val = dp_cur[p];
            if (val == (seq_t)0) continue;
            uint8_t len = g_edges_len[p];
            if (len == 0) continue;
            int prev = p % NROWS;  /* last row in the pair */
            for (uint8_t k = 0; k < len; ++k) {
                int next_row = g_edges[p][k];
                size_t next_idx = (size_t)prev * NROWS + next_row;
                dp_next[next_idx] += val;
            }
        }

        /* Swap arrays for the next iteration */
        for (size_t i = 0; i < NP; ++i) dp_cur[i] = dp_next[i];
    }

    /* Result: sum over all final states */
    seq_t res = (seq_t)0;
    for (size_t i = 0; i < NP; ++i) res += dp_cur[i];
    return res;
}
