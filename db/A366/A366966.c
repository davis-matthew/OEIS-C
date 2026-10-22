/*
 * A366966  Numbers whose difference between the largest and smallest digits
 *          is equal to 9.
 * https://oeis.org/A366966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal representation *
 *            contains both digits 0 and 9. (These are the numbers counted by
 *            * A366966.)
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t  count = 0;

    while (count < n) {
        val++;                      /* next integer to test          */
        seq_t tmp = val;
        bool has0 = false;
        bool has9 = false;

        while (tmp > 0) {
            seq_t digit = tmp % 10;
            if (digit == 0) has0 = true;
            else if (digit == 9) has9 = true;
            if (has0 && has9) break;
            tmp /= 10;
        }

        if (has0 && has9) count++;
    }

    return val;
}
