/*
 * A260522  Concatenate the positions of digits 9, 8,..., 0 in the decimal
 *          representation of n, using 1 for the leftmost digit etc., and 0
 *          when the digit does not occur.
 * https://oeis.org/A260522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n, for each decimal digit d from 9 down to 0, take the
 *            positions (counted from left, starting at 1) where d occurs in
 *            n. Concatenate these positions for each d; if d does not occur,
 *            use a single digit 0. The resulting decimal string is converted
 *            to a number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* obtain decimal digits of n (leftmost first) */
    int digits[64];
    idx_t tmp = n;
    int len = 0;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp) {
            digits[len++] = (int)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to get left‑to‑right order */
        for (int i = 0; i < len / 2; ++i) {
            int swap = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = swap;
        }
    }

    /* store positions of each digit */
    int pos[10][64];
    int cnt[10] = {0};

    for (int i = 0; i < len; ++i) {
        int d = digits[i];
        pos[d][cnt[d]++] = i + 1;   /* positions are 1‑based */
    }

    /* build result by appending position digits */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        if (cnt[d] == 0) {
            result = result * 10 + 0;
        } else {
            for (int i = 0; i < cnt[d]; ++i) {
                result = result * 10 + pos[d][i];
            }
        }
    }

    return result;
}
