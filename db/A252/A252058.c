/*
 * A252058  Number of (n+2)X(1+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 2 or 4.
 * https://oeis.org/A252058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2) × 3 arrays over {0,1,2} with every 3×3
 *            subblock (row‑wise) satisfying that each row, column, main
 *            diagonal and anti‑diagonal sum is *not* equal to 1, 2 or 4. The
 *            values are computed by dynamic programming over triples of rows
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 21, u64 n <= 47, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------------- */
    /* Precompute validity of every 3‑row triple.  Index of a row is the
       3‑digit base‑3 number:  row[i][j] = (row_num / 3^(2-j)) % 3          */
    static uint8_t  valid[27*27*27];
    static int      digits[27][3];
    static int      init = 0;
    if (!init) {
        for (int i = 0; i < 27; ++i) {
            int v = i;
            digits[i][0] = v / 9;
            v %= 9;
            digits[i][1] = v / 3;
            digits[i][2] = v % 3;
        }
        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                for (int c = 0; c < 27; ++c) {
                    bool ok = true;
                    /* row sums */
                    int rsum;
                    rsum = digits[a][0] + digits[a][1] + digits[a][2];
                    if (rsum == 1 || rsum == 2 || rsum == 4) { ok = false; }
                    if (ok) {
                        rsum = digits[b][0] + digits[b][1] + digits[b][2];
                        if (rsum == 1 || rsum == 2 || rsum == 4) { ok = false; }
                    }
                    if (ok) {
                        rsum = digits[c][0] + digits[c][1] + digits[c][2];
                        if (rsum == 1 || rsum == 2 || rsum == 4) { ok = false; }
                    }
                    /* column sums */
                    if (ok) {
                        for (int j = 0; j < 3; ++j) {
                            int cs = digits[a][j] + digits[b][j] + digits[c][j];
                            if (cs == 1 || cs == 2 || cs == 4) { ok = false; break; }
                        }
                    }
                    /* main diagonal */
                    if (ok) {
                        int md = digits[a][0] + digits[b][1] + digits[c][2];
                        if (md == 1 || md == 2 || md == 4) { ok = false; }
                    }
                    /* anti diagonal */
                    if (ok) {
                        int ad = digits[a][2] + digits[b][1] + digits[c][0];
                        if (ad == 1 || ad == 2 || ad == 4) { ok = false; }
                    }
                    valid[(a*27 + b)*27 + c] = ok ? 1 : 0;
                }
            }
        }
        init = 1;
    }

    /* -------------------------------------------------------------------- */
    /* Dynamic programming over rows.  prev[pair_id] holds the number of
       arrays ending with the last two rows = pair_id.  Each transition
       adds a new row and checks the 3×3 block constraint. */
    static seq_t *prev = NULL;
    static seq_t *cur  = NULL;
    static int  allocated = 0;
    if (!allocated) {
        prev = (seq_t*)malloc(27*27*sizeof(seq_t));
        cur  = (seq_t*)malloc(27*27*sizeof(seq_t));
        allocated = 1;
    }

    if (n == 0) return (seq_t)0;          /* not part of the sequence */

    size_t rows   = (size_t)n + 2;        /* total rows of the array */
    size_t state  = 27 * 27;              /* number of state pairs */

    /* initialise for the first two rows: any pair is possible */
    for (size_t i = 0; i < state; ++i) prev[i] = (seq_t)1;

    /* perform n transition steps (adding a new row each time) */
    for (size_t t = 3; t <= rows; ++t) {
        for (size_t i = 0; i < state; ++i) cur[i] = (seq_t)0;

        for (int b = 0; b < 27; ++b) {
            for (int c = 0; c < 27; ++c) {
                seq_t sum = (seq_t)0;
                for (int a = 0; a < 27; ++a) {
                    if (valid[(a*27 + b)*27 + c])
                        sum += prev[a*27 + b];
                }
                cur[b*27 + c] = sum;
            }
        }
        /* swap prev and cur for next iteration */
        seq_t *tmp = prev; prev = cur; cur = tmp;
    }

    /* final answer: sum over all ending pair states */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < state; ++i) total += prev[i];
    return total;
}
