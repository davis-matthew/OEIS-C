/*
 * A260529  Concatenate the positions of digits 9, 8,..., 0 in the decimal
 *          representation of n, using 1 for the rightmost digit etc., skip it
 *          if a digit does not occur.
 * https://oeis.org/A260529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Concatenate the positions of digits 9,8,…,0 in the decimal
 *            representation * of n, using 1 for the rightmost digit; skip
 *            digits that do not occur
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;                     /* final concatenated number */

    /* for each digit from 9 down to 0 */
    for (int d = 9; d >= 0; d--) {
        idx_t tmp = n;                    /* copy of n for scanning */
        idx_t pos = 1;                    /* position counter (rightmost is 1) */

        /* handle the special case n == 0 (single digit '0') */
        if (tmp == 0) {
            if (d == 0) {
                seq_t p = 1;              /* position is 1 */
                seq_t shift = 10;         /* one decimal digit */
                result = result * shift + p;
            }
            continue;
        }

        /* scan all decimal digits from least significant to most */
        while (tmp > 0) {
            int cur = (int)(tmp % 10);    /* current digit, 0..9 */
            if (cur == d) {
                seq_t p = (seq_t)pos;     /* position value */

                /* compute 10^digits(p) with a small loop */
                seq_t shift = 1;
                idx_t pp = pos;
                while (pp > 0) {
                    shift *= 10;
                    pp /= 10;
                }

                result = result * shift + p;  /* append the decimal representation */
            }
            tmp /= 10;
            pos++;
        }
    }

    return result;
}
