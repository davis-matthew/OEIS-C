/*
 * A253657  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum unequal to its neighbors
 *          horizontally, vertically and nw-se diagonally.
 * https://oeis.org/A253657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays of digits 0,1,2 satisfying the
 *            D‑difference adjacency condition (computed by dynamic
 *            programming over successive rows)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/* Precomputation tables (static so they are computed only once) -------- */
static bool pair_ok[81][81];                   /* horizontal adjacency OK */
static signed char diff_val[81][81][3];        /* D values for a pair */
static unsigned char compat_list[81][81][81];  /* list of compatible a's */
static unsigned char compat_len[81][81];       /* length of each list   */
static int rowv[81][4];                       /* digits of each row */
static bool precomp_done = false;

/* ---------------------------------------------------------------------- */
static void precompute(void)
{
    if (precomp_done) return;
    precomp_done = true;

    /* Generate all 81 possible rows (base‑3 representation) */
    for (int r = 0; r < 81; ++r) {
        int x = r;
        for (int c = 0; c < 4; ++c) {
            rowv[r][c] = x % 3;
            x /= 3;
        }
    }

    /* Determine properties of each pair of rows */
    for (int a = 0; a < 81; ++a) {
        for (int b = 0; b < 81; ++b) {
            signed char d[3];
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                int diag = (rowv[a][j] > rowv[b][j + 1]) ? rowv[a][j] : rowv[b][j + 1];
                int anti = (rowv[a][j + 1] > rowv[b][j]) ? rowv[a][j + 1] : rowv[b][j];
                d[j] = (signed char)(diag - anti);
            }
            if (d[0] == d[1] || d[1] == d[2]) ok = false;
            pair_ok[a][b] = ok;
            if (ok) {
                diff_val[a][b][0] = d[0];
                diff_val[a][b][1] = d[1];
                diff_val[a][b][2] = d[2];
            }
        }
    }

    /* Precompute compatibility lists: for every pair (b,c) store
       all a such that pair (a,b) can precede pair (b,c). */
    for (int b = 0; b < 81; ++b) {
        for (int c = 0; c < 81; ++c) {
            if (!pair_ok[b][c]) continue;
            int len = 0;
            for (int a = 0; a < 81; ++a) {
                if (!pair_ok[a][b]) continue;
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    if (diff_val[a][b][j] == diff_val[b][c][j]) { ok = false; break; }
                    if (j < 2 && diff_val[a][b][j] == diff_val[b][c][j + 1]) { ok = false; break; }
                }
                if (ok) {
                    compat_list[b][c][len++] = (unsigned char)a;
                }
            }
            compat_len[b][c] = (unsigned char)len;
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset < 1 */
    precompute();

    /* DP tables: dp[a][b] = number of partial arrays ending with pair (a,b) */
    seq_t dp[81][81];
    seq_t ndp[81][81];

    /* Initialise for height 2 rows (n == 1) */
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            dp[i][j] = 0;
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            if (pair_ok[i][j])
                dp[i][j] = 1;

    /* Append rows up to height n+1 (= n+1 rows) */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                ndp[i][j] = 0;

        for (int j = 0; j < 81; ++j) {          /* previous second row = j */
            for (int k = 0; k < 81; ++k) {      /* current second row = k */
                if (!pair_ok[j][k]) continue;
                int len = compat_len[j][k];
                unsigned char *list = compat_list[j][k];
                for (int t = 0; t < len; ++t) {
                    int a = list[t];
                    ndp[j][k] += dp[a][j];
                }
            }
        }

        /* Copy new counts into dp for next iteration */
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all terminal states to obtain the answer */
    seq_t result = 0;
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            result += dp[i][j];

    return result;
}
