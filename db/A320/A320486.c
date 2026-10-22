/*
 * A320486  Keep just the digits of n that appear exactly once; write 0 if all
 *          digits disappear.
 * https://oeis.org/A320486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = keep digits that occur exactly once in decimal representation of n, otherwise 0
   leading zeros eliminated unless all digits are removed (in that case 0 is returned) */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    /* Count frequency of each decimal digit in n */
    unsigned char freq[10] = {0};
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        freq[d]++;
        tmp /= 10;
    }

    /* Store digits in reverse order to allow forward traversal */
    unsigned char rev[20]; /* maximum 20 decimal digits for 64‑bit n */
    int len = 0;
    tmp = n;
    while (tmp > 0) {
        rev[len++] = (unsigned)(tmp % 10);
        tmp /= 10;
    }

    /* Build the result from most significant digit to least */
    seq_t result = 0;
    bool leading = true;
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = rev[i];
        if (freq[d] == 1) {
            if (leading && d == 0) {
                /* Skip leading zero */
                continue;
            }
            leading = false;
            result = result * 10 + d;
        }
    }

    /* If no digit was kept, result is 0 (already the case) */
    return result;
}
