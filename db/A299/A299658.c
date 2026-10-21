/*
 * A299658  Number of nX5 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over 5‑column binary *
 *            arrays with king‑move adjacency constraints. The state for each
 *            * row is a 5‑bit vector (32 possibilities). For n≥3 we maintain
 *            * transitions from (prev, curr) rows to (curr, next) rows,
 *            checking * that the middle row satisfies the co...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#ifndef WIDTH
#define WIDTH 5
#endif
#ifndef STATE
#define STATE (1 << WIDTH)  /* 32 */
#endif

/* Precompute whether a triple of consecutive rows is valid for the
 * middle row.  valid[a][b][c] is true iff the middle row 'b' obeys
 * the rule when surrounded by rows 'a' (above) and 'c' (below). */
static bool valid[STATE][STATE][STATE];
static bool valid_precomputed = false;

static void precompute_valid(void)
{
    if (valid_precomputed) return;
    for (int a = 0; a < STATE; ++a) {
        for (int b = 0; b < STATE; ++b) {
            for (int c = 0; c < STATE; ++c) {
                bool ok = true;
                for (int j = 0; j < WIDTH; ++j) {
                    int v = (b >> j) & 1;
                    int cnt = 0;
                    /* neighbours from row a (above) */
                    if (j > 0)   if (((a >> (j-1)) & 1) == v) ++cnt;
                    if (((a >> j) & 1) == v)                 ++cnt;
                    if (j < WIDTH-1) if (((a >> (j+1)) & 1) == v) ++cnt;
                    /* neighbours from row b (same row) */
                    if (j > 0)   if (((b >> (j-1)) & 1) == v) ++cnt;
                    if (j < WIDTH-1) if (((b >> (j+1)) & 1) == v) ++cnt;
                    /* neighbours from row c (below) */
                    if (j > 0)   if (((c >> (j-1)) & 1) == v) ++cnt;
                    if (((c >> j) & 1) == v)                 ++cnt;
                    if (j < WIDTH-1) if (((c >> (j+1)) & 1) == v) ++cnt;
                    if (cnt == 6 || cnt == 8) { ok = false; break; }
                }
                valid[a][b][c] = ok;
            }
        }
    }
    valid_precomputed = true;
}

seq_t a(idx_t n)
{
    precompute_valid();

    /* Small heights with no interior rows. */
    if (n == 1) return (seq_t)16; /* 2^(5-1) */
    if (n == 2) return (seq_t)512;/* 2^(10-1) */

    /* dp[p][c] = number of ways after processing current row 'c'
     * with the previous row being 'p'.  We use two 32×32 tables. */
    static seq_t dp[STATE][STATE];
    static seq_t ndp[STATE][STATE];

    seq_t zero = 0, one = 1;
    /* initialise dp to zero */
    for (int i = 0; i < STATE; ++i)
        for (int j = 0; j < STATE; ++j)
            dp[i][j] = zero;

    /* Initial two rows: top row with bit 0 fixed to 0,
     * second row free. */
    for (int r1 = 0; r1 < STATE; ++r1) {
        if ((r1 & 1) == 0) {                /* upper-left element zero */
            for (int r2 = 0; r2 < STATE; ++r2)
                dp[r1][r2] = one;
        }
    }

    /* Process rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < STATE; ++i)
            for (int j = 0; j < STATE; ++j)
                ndp[i][j] = zero;

        for (int a = 0; a < STATE; ++a) {
            for (int b = 0; b < STATE; ++b) {
                seq_t cur = dp[a][b];
                if (cur == zero) continue;   /* skip empty states */
                for (int c = 0; c < STATE; ++c) {
                    if (valid[a][b][c])
                        ndp[b][c] += cur;
                }
            }
        }
        /* copy ndp into dp for next iteration */
        for (int i = 0; i < STATE; ++i)
            for (int j = 0; j < STATE; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all final states.  No constraint for the last row. */
    seq_t total = zero;
    for (int a = 0; a < STATE; ++a)
        for (int b = 0; b < STATE; ++b)
            total += dp[a][b];

    return total;
}
