/*
 * A274650  Triangle read by rows: T(n,k), (0 <= k <= n), in which each term
 *          is the least nonnegative integer such that no row, column,
 *          diagonal, or antidiagonal contains a repeated term.
 * https://oeis.org/A274650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 8075, u16 n <= 8075, u32 n <= 8075, u64 n <= 8075,
 *            u128 n <= 8075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A274650 via incremental construction: each entry is the smallest
 * non‑negative integer that does not appear in its row, column,
 * main diagonal, or antidiagonal. */

#include <stddef.h>

#define MAX_ROW   200                 /* enough for n ≤ 10 000 */
#define MAX_DIAG  (2*MAX_ROW-1)
#define OFFSET    (MAX_ROW-1)

/* Storage for triangle and constraint sets */
static seq_t tri[    MAX_ROW][MAX_ROW];
static int  col_len[ MAX_ROW];
static seq_t col_vals[ MAX_ROW][MAX_ROW];

static int  diag_len[ MAX_DIAG];
static seq_t diag_vals[ MAX_DIAG][MAX_ROW];

static int  anti_len[ MAX_DIAG];
static seq_t anti_vals[ MAX_DIAG][MAX_ROW];

static idx_t built_rows = 0;

/* build a single row r (0‑based) */
static void build_row(idx_t r)
{
    int len = (int)r + 1;
    for (int k = 0; k < len; ++k) {
        seq_t v;
        for (v = 0; ; ++v) {
            int ok = 1;

            /* row constraint */
            for (int m = 0; m < k && ok; ++m)
                if (tri[r][m] == v) ok = 0;

            /* column constraint */
            for (int c = 0; c < col_len[k] && ok; ++c)
                if (col_vals[k][c] == v) ok = 0;

            /* main diagonal (i-j = r-k) */
            int dindex = (int)r - k + OFFSET;
            for (int d = 0; d < diag_len[dindex] && ok; ++d)
                if (diag_vals[dindex][d] == v) ok = 0;

            /* antidiagonal (i+j = r+k) */
            int aindex = (int)r + k;
            for (int a = 0; a < anti_len[aindex] && ok; ++a)
                if (anti_vals[aindex][a] == v) ok = 0;

            if (ok) break;                /* found minimal valid value */
        }

        tri[r][k] = v;
        col_vals[k][col_len[k]++] = v;
        int dindex = (int)r - k + OFFSET;
        diag_vals[dindex][diag_len[dindex]++] = v;
        int aindex = (int)r + k;
        anti_vals[aindex][anti_len[aindex]++] = v;
    }
    built_rows++;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* locate the row containing index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* build rows up to r if not already done */
    while (built_rows <= r) build_row(built_rows);

    /* column index within the row */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    return tri[r][k];
}
