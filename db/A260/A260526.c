/*
 * A260526  Concatenate the positions of digits 0, 1,..., 9 in the decimal
 *          representation of n, using 1 for the rightmost digit etc., skip it
 *          if a digit does not occur.
 * https://oeis.org/A260526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate, for digits d=0..9, the decimal positions
 *            (from the right, position 1) of occurrences of d in the decimal
 *            representation of n; skip a digit if it does not occur. The
 *            concatenation writes positions for each digit in ascending order
 *            of the positions
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum decimal digits for a 64‑bit integer is 20 */
    enum { MAX_DIGS = 20 };
    idx_t pos_array[10][MAX_DIGS];
    int count[10] = {0};

    if (n == 0) {
        /* special case: decimal "0" → digit 0 at position 1 */
        pos_array[0][0] = 1;
        count[0] = 1;
    } else {
        idx_t temp = n;
        idx_t pos = 1;
        while (temp > 0) {
            int d = (int)(temp % 10);
            pos_array[d][count[d]++] = pos;
            temp /= 10;
            ++pos;
        }
    }

    seq_t res = 0;
    char digit_buf[20];
    for (int d = 0; d <= 9; ++d) {
        for (int i = 0; i < count[d]; ++i) {
            idx_t p = pos_array[d][i];
            /* convert position p to decimal digits */
            int len = 0;
            idx_t tmp = p;
            do {
                digit_buf[len++] = (char)('0' + (tmp % 10));
                tmp /= 10;
            } while (tmp > 0);
            /* append digits in correct order */
            for (int k = len - 1; k >= 0; --k) {
                res = res * 10 + (seq_t)(digit_buf[k] - '0');
            }
        }
    }
    return res;
}
