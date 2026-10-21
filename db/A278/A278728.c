/*
 * A278728  Number of n X 2 0..3 arrays with rows in nondecreasing
 *          lexicographic order and columns in nonincreasing lexicographic
 *          order, but with exactly one mistake.
 * https://oeis.org/A278728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 42, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic‑programming count of n×2 arrays over {0,1,2,3}
   with rows in nondecreasing lexicographic order and columns in
   nonincreasing lexicographic order, but with exactly one violation
   (a single mistake).  The program keeps, for each prefix of rows,
   the last row, the number of row violations so far, and the current
   state of the column lexicographic comparison (no difference seen,
   first difference a>b, first difference a<b).  After processing n rows
   it sums the states whose total number of mistakes equals one.  This
   yields the desired OEIS sequence A278728. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* states: viol=0/1, col_state=0/1/2, last_row=0..15 (a*4+b) */
    seq_t cur[2][3][16];
    seq_t nxt[2][3][16];

    /* initialise to zero */
    for (int v = 0; v < 2; ++v)
        for (int s = 0; s < 3; ++s)
            for (int r = 0; r < 16; ++r)
                cur[v][s][r] = 0;

    /* first row: no row violation yet */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b) {
            int idx = a * 4 + b;
            int cs = 0;                /* column state */
            if (a > b) cs = 1;
            else if (a < b) cs = 2;
            cur[0][cs][idx] = cur[0][cs][idx] + 1;
        }

    /* process remaining rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next */
        for (int v = 0; v < 2; ++v)
            for (int s = 0; s < 3; ++s)
                for (int r = 0; r < 16; ++r)
                    nxt[v][s][r] = 0;

        for (int viol = 0; viol < 2; ++viol) {
            for (int cs = 0; cs < 3; ++cs) {
                for (int r = 0; r < 16; ++r) {
                    seq_t cnt = cur[viol][cs][r];
                    if (cnt == 0) continue;
                    int a_prev = r / 4;
                    int b_prev = r % 4;

                    for (int a = 0; a < 4; ++a)
                        for (int b = 0; b < 4; ++b) {
                            int r2 = a * 4 + b;
                            int row_viol = (a_prev > a) || (a_prev == a && b_prev > b);
                            int new_viol = viol + row_viol;
                            if (new_viol > 1) continue;

                            int new_cs = cs;
                            if (cs == 0) {
                                if (a > b) new_cs = 1;
                                else if (a < b) new_cs = 2;
                            }

                            nxt[new_viol][new_cs][r2] = nxt[new_viol][new_cs][r2] + cnt;
                        }
                }
            }
        }

        /* swap cur and nxt */
        for (int v = 0; v < 2; ++v)
            for (int s = 0; s < 3; ++s)
                for (int r = 0; r < 16; ++r)
                    cur[v][s][r] = nxt[v][s][r];
    }

    /* sum states with exactly one total mistake */
    seq_t ans = 0;
    for (int viol = 0; viol < 2; ++viol) {
        for (int cs = 0; cs < 3; ++cs) {
            for (int r = 0; r < 16; ++r) {
                seq_t cnt = cur[viol][cs][r];
                if (cnt == 0) continue;
                if (cs == 2 && viol == 0)          /* column violation only */
                    ans = ans + cnt;
                if (cs != 2 && viol == 1)           /* one row violation only */
                    ans = ans + cnt;
            }
        }
    }
    return ans;
}
