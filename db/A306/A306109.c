/*
 * A306109  Numbers with digits in {0,...,6} such that every other digit is
 *          strictly less than its neighbors.
 * https://oeis.org/A306109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Brute‑force enumeration of decimal numbers with digits 0‑6 *
 *            where each even‑position digit is strictly smaller than its
 *            neighbours
 * Range:     u8 n <= 45, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 0;

    while (1) {
        seq_t tmp = x;
        uint8_t digits[32];
        int len = 0;
        bool ok_digits = true;

        /* Extract decimal digits, checking the 0‑6 restriction. */
        if (tmp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (tmp > 0) {
                uint8_t d = (uint8_t)(tmp % 10);
                if (d >= 7) {          /* digit too large */
                    ok_digits = false;
                    break;
                }
                digits[len++] = d;
                tmp /= 10;
            }
        }

        if (!ok_digits) {
            ++x;
            continue;
        }

        /* Verify the valley property on even positions (1‑based from left). */
        bool ok = true;
        for (int j = 0; j < len; ++j) {
            int pos = len - j;           /* position from left, 1‑based */
            if ((pos & 1) == 0) {        /* even position */
                uint8_t d = digits[j];
                if (j > 0 && d >= digits[j - 1]) {   /* left neighbor */
                    ok = false;
                    break;
                }
                if (j + 1 < len && d >= digits[j + 1]) { /* right neighbor */
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            ++found;
            if (found == n)
                return x;
        }

        ++x;
    }
}
