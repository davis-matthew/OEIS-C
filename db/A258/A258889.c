/*
 * A258889  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum or central row sum less than
 *          the central column sum.
 * https://oeis.org/A258889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 31, u64 n <= 77, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258889 - number of (n+2)×5 binary arrays with the property that
 * in every 3×3 subblock the main‑diagonal sum is at least the anti‑diagonal
 * sum and the sum of the central row is at least the sum of the central
 * column.  The function returns a(n) for n ≥ 1.
 *
 * This implementation uses dynamic programming over the rows.  The state
 * is the last two rows (each represented as a 5‑bit number).  For each
 * possible new row we pre‑compute whether the 3×3 condition would be
 * satisfied.  Then we iterate n times (adding the third, fourth, …,
 * (n+2)‑nd row) and accumulate the counts.  Complexity is O(n); the
 * transition table has size 32×32×32 = 32768.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* transition table: trans[(a*32)+b][c] == 1 if the rows a,b,c satisfy
 * the 3×3 condition for all sliding windows; 0 otherwise. */
static unsigned char trans[1024][32];
static int trans_inited = 0;

/* Precompute all transitions lazily. */
static void init_trans(void)
{
    if (trans_inited)
        return;
    for (int a = 0; a < 32; ++a) {
        int a0 =  (a >> 0) & 1, a1 = (a >> 1) & 1, a2 = (a >> 2) & 1,
            a3 = (a >> 3) & 1, a4 = (a >> 4) & 1;
        for (int b = 0; b < 32; ++b) {
            int b0 =  (b >> 0) & 1, b1 = (b >> 1) & 1, b2 = (b >> 2) & 1,
                b3 = (b >> 3) & 1, b4 = (b >> 4) & 1;
            for (int c = 0; c < 32; ++c) {
                int c0 =  (c >> 0) & 1, c1 = (c >> 1) & 1, c2 = (c >> 2) & 1,
                    c3 = (c >> 3) & 1, c4 = (c >> 4) & 1;
                int ok = 1;
                /* Check the three sliding 3×3 windows (j = 0,1,2). */
                for (int j = 0; j < 3 && ok; ++j) {
                    int a_col_j,  a_col_j1,  a_col_j2;
                    int b_col_j,  b_col_j1,  b_col_j2;
                    int c_col_j,  c_col_j1,  c_col_j2;

                    if (j == 0) {
                        a_col_j = a0;  a_col_j1 = a1;  a_col_j2 = a2;
                        b_col_j = b0;  b_col_j1 = b1;  b_col_j2 = b2;
                        c_col_j = c0;  c_col_j1 = c1;  c_col_j2 = c2;
                    } else if (j == 1) {
                        a_col_j = a1;  a_col_j1 = a2;  a_col_j2 = a3;
                        b_col_j = b1;  b_col_j1 = b2;  b_col_j2 = b3;
                        c_col_j = c1;  c_col_j1 = c2;  c_col_j2 = c3;
                    } else { /* j == 2 */
                        a_col_j = a2;  a_col_j1 = a3;  a_col_j2 = a4;
                        b_col_j = b2;  b_col_j1 = b3;  b_col_j2 = b4;
                        c_col_j = c2;  c_col_j1 = c3;  c_col_j2 = c4;
                    }

                    int diag   = a_col_j + b_col_j1 + c_col_j2;
                    int anti   = a_col_j2 + b_col_j1 + c_col_j;
                    if (diag < anti)
                        { ok = 0; break; }

                    int row_sum = b_col_j + b_col_j1 + b_col_j2;
                    int col_sum = a_col_j1 + b_col_j1 + c_col_j1;
                    if (row_sum < col_sum)
                        { ok = 0; break; }
                }
                trans[(a * 32) + b][c] = (unsigned char)ok;
            }
        }
    }
    trans_inited = 1;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but we return 0 safely for n==0. */
    if (n == 0)
        return 0;

    init_trans();

    /* Number of transitions equals number of rows after the first two. */
    size_t steps = (size_t)n;   /* n rows beyond the first two */

    seq_t dp[1024];
    seq_t dp2[1024];

    /* Initialise all 32×32 starting states to 1. */
    for (int i = 0; i < 1024; ++i)
        dp[i] = 1;

    for (size_t step = 0; step < steps; ++step) {
        /* Zero dp2 for arbitrary seq_t types. */
        for (int i = 0; i < 1024; ++i)
            dp2[i] = 0;

        for (int a = 0; a < 32; ++a) {
            int base_a = a << 5;  /* a * 32 */
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp[base_a + b];
                if (val == 0)
                    continue;
                const unsigned char *next = trans[base_a + b];
                int base_b = b << 5;  /* b * 32 */
                for (int c = 0; c < 32; ++c) {
                    if (next[c])
                        dp2[base_b + c] += val;
                }
            }
        }

        /* Swap dp and dp2 by copying. */
        for (int i = 0; i < 1024; ++i)
            dp[i] = dp2[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 1024; ++i)
        total += dp[i];

    return total;
}
