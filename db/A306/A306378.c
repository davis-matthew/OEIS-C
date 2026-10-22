/*
 * A306378  Number of steps of applying "Erase or triple" protocol to n before
 *          reaching 0, 5, 7, or 89.
 * https://oeis.org/A306378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of steps of the "Erase or triple" protocol until the
 * number reaches one of the four loops 0, 5, 7 or 89.
 * The protocol is:
 *   • Count the decimal digits of the current number.
 *   • Let m be the maximal frequency of any digit.
 *   • If m == 1 (all digits are distinct) then replace the number by 3*number.
 *   • Otherwise delete all occurrences of every digit that occurs exactly m times.
 *     (If several digits tie for the maximum, all of them are deleted.)
 *   • If the deletion removes all digits the new number becomes 0.
 *   The process stops when the number is 0, 5, 7 or 89, and the return
 *   value is the count of transitions performed. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* If already in a loop, 0 steps. */
    if (x == 0 || x == 5 || x == 7 || x == 89)
        return 0;

    idx_t steps = 0;
    while (1) {
        /* Stop when a loop is reached. */
        if (x == 0 || x == 5 || x == 7 || x == 89)
            break;

        /* Count digit frequencies. */
        int freq[10] = {0};
        seq_t t = x;
        if (t == 0) {
            freq[0] = 1;
        } else {
            while (t > 0) {
                int d = (int)(t % 10);
                freq[d]++;
                t /= 10;
            }
        }

        /* Determine maximal frequency. */
        int mf = 0;
        for (int d = 0; d < 10; ++d)
            if (freq[d] > mf)
                mf = freq[d];

        if (mf == 1) {
            /* All digits distinct: triple the number. */
            x = x * 3;
        } else {
            /* Delete all digits that occur mf times. */
            bool del[10] = {0};
            for (int d = 0; d < 10; ++d)
                if (freq[d] == mf)
                    del[d] = true;

            /* Extract digits into an array (least‑to‑most). */
            uint8_t digs[200];
            int len = 0;
            t = x;
            if (t == 0) {
                digs[len++] = 0;
            } else {
                while (t > 0) {
                    digs[len++] = (uint8_t)(t % 10);
                    t /= 10;
                }
            }

            /* Rebuild the number from most‑to‑least with deletions. */
            seq_t newx = 0;
            for (int i = len - 1; i >= 0; --i) {
                int d = digs[i];
                if (del[d])
                    continue;
                newx = newx * 10 + d;
            }
            x = newx;
        }

        ++steps;
    }

    return (seq_t)steps;
}
