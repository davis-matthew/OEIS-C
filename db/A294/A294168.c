/*
 * A294168  Irregular triangle read by rows in which row n contains
 *          significant digits after the radix point for unit fractions 1/n
 *          expanded in factorial base.
 * https://oeis.org/A294168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8825, u16 n <= 8825, u32 n <= 8825, u64 n <= 8825,
 *            u128 n <= 8825, big any n
 * Helpers:   minimal_factorial_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/minimal_factorial_multiple.h"
/* Computes A294168: the nth term is the nth digit obtained by concatenating
 * the factorial‑base digits of 1/k for k ≥ 1 (row 1 contains the single digit 0). */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but guard against underflow */
        return (seq_t)0;

    idx_t pos = n - 1;   /* zero‑based position in the infinite list */
    idx_t row = 1;

    while (1) {
        idx_t row_len;

        if (row == 1) {
            row_len = 1;        /* row 1 contains only the digit 0 */
            if (pos == 0)
                return (seq_t)0;
        } else {
            idx_t m = minimal_factorial_multiple(row);
            row_len = m - 1;    /* number of digits in this row */

            if (pos < row_len) {
                /* Compute the (pos)‑th digit of this row.  For 1/row, the
                 * factorial‑base digits a_k satisfy: a_k = floor(rem * k / row)
                 * where rem is updated as rem = (rem * k) % row, starting
                 * with rem = 1.  The algorithm runs in O(row_len). */
                idx_t rem = 1;
                seq_t nn = (seq_t)row;

                for (idx_t k = 2; k <= m; ++k) {
                    seq_t digit = (seq_t)((rem * k) / row);
                    if (pos == 0)
                        return digit;
                    --pos;
                    rem = (rem * k) % row;
                }
                /* The loop must return; fall‑through is impossible. */
            }
        }

        /* Skip this row and continue with the next. */
        pos -= row_len;
        ++row;
    }
}
