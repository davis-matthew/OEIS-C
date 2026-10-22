/*
 * A345972  Numbers that are integer multiples of the count of active segments
 *          in their 7-segment-display form where '6', '7' and '9' use 6, 3
 *          and 6 segments, respectively.
 * https://oeis.org/A345972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345972: numbers that are integer multiples of the count of active
   segments in their 7‑segment display form, enumerated in increasing order.
   Function returns the nth such number (offset 1). */
#include <stdbool.h>

static const uint8_t segcnt[10] = {
    6, /* 0 */
    2, /* 1 */
    5, /* 2 */
    5, /* 3 */
    4, /* 4 */
    5, /* 5 */
    6, /* 6 */
    3, /* 7 */
    7, /* 8 */
    6  /* 9 */
};

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t m = 0;

    while (true) {
        /* compute sum of segment counts for m */
        seq_t sum = 0;
        seq_t tmp = m;
        if (tmp == 0) {
            sum += segcnt[0];
        } else {
            while (tmp > 0) {
                unsigned int digit = (unsigned int)(tmp % 10);
                sum += segcnt[digit];
                tmp /= 10;
            }
        }

        if (m % sum == 0) {
            ++found;
            if (found == n) return m;
        }
        ++m;
    }
}
