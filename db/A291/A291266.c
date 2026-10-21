/*
 * A291266  Numbers that contain at least 3 identical digits.
 * https://oeis.org/A291266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that contains at least three
 *            identical decimal digits. The function searches for the next
 *            candidate until the n-th one is found
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integral operations supported by seq_t
   and idx_t, without any floating point or external helpers. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  // offset 1; n>=1 in all meaningful calls

    idx_t found = 0;       // number of qualifying integers already found
    idx_t current = 1;     // current integer to test

    while (true) {
        seq_t val = (seq_t)current;
        uint8_t digit_count[10] = {0};
        seq_t temp = val;
        bool qualifies = false;

        /* Count digits; stop as soon as any digit appears at least 3 times */
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            if (++digit_count[digit] >= 3) {
                qualifies = true;
                break;
            }
            temp /= 10;
        }

        if (qualifies) {
            found++;
            if (found == n) {
                return val;  // the n-th qualifying number
            }
        }

        current++;  /* advance to the next integer */
    }
}
