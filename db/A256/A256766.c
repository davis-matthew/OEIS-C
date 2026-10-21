/*
 * A256766  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum.
 * https://oeis.org/A256766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic‑programming count of (n+2)×5 binary matrices such that
 *            for every 3×3 subblock the diagonal sum is ≥ the anti‑diagonal
 *            sum
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    /* ------------------------------------------------------------------
       Transition mask: next_mask[a][b] has bit c set iff rows a,b,c satisfy
       the 3×3 constraints for all column positions j = 0,1,2.
       ------------------------------------------------------------------ */
    static uint32_t next_mask[32][32];
    static int masks_ready = 0;
    if (!masks_ready) {
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                uint32_t mask = 0;
                for (int c = 0; c < 32; ++c) {
                    int ok = 1;
                    for (int j = 0; j < 3; ++j) {
                        int a_bit      = (a >> j)       & 1;
                        int a_bit_plus2  = (a >> (j + 2)) & 1;
                        int c_bit      = (c >> j)       & 1;
                        int c_bit_plus2  = (c >> (j + 2)) & 1;
                        if ((a_bit - a_bit_plus2 + c_bit_plus2 - c_bit) < 0) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) mask |= 1u << c;
                }
                next_mask[a][b] = mask;
            }
        }
        masks_ready = 1;
    }

    /* ------------------------------------------------------------------
       DP tables: dp[a][b] counts arrays ending with rows a (previous‑previous)
       and b (previous) after the current number of processed rows.
       ------------------------------------------------------------------ */
    static seq_t dp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)1;          /* all 32×32 pairs allowed for the first two rows */

    idx_t total_rows = n + 2;             /* number of rows in the matrix */

    /* Each new row extends the matrix by one.  For rows 3 … total_rows
       we update the DP according to the precomputed transition mask. */
    for (idx_t row = 3; row <= total_rows; ++row) {
        seq_t newdp[32][32];
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = (seq_t)0;

        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp[a][b];
                if (val == (seq_t)0) continue;
                uint32_t mask = next_mask[a][b];
                for (int c = 0; c < 32; ++c)
                    if (mask & (1u << c))
                        newdp[b][c] += val;
            }

        /* copy newdp back into dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over all final pairs to obtain a(n). */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            result += dp[i][j];

    return result;
}
