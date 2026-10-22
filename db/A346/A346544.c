/*
 * A346544  Indices of repeated digits of Champernowne's constant.
 * https://oeis.org/A346544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute the n-th index where a digit repeats in Champernowne's
   decimal constant (numbers 1,2,3,…) by scanning its digits from the
   start and counting positions where a digit equals the immediately
   preceding one. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;          /* undefined for n=0, return 0 */
    }

    uint64_t pos = 1;              /* current position in Champernowne's digits (1‑based) */
    int prev_digit = -1;           /* previous digit, -1 means none yet */
    uint64_t repeats = 0;          /* how many repeats found so far */

    for (uint64_t number = 1; ; ++number) {
        /* extract decimal digits of 'number' into buffer in reverse order */
        char buf[32];
        int len = 0;
        uint64_t tmp = number;
        do {
            buf[len++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        } while (tmp > 0);

        /* iterate digits from most significant to least */
        for (int i = len - 1; i >= 0; --i) {
            int d = buf[i] - '0';
            if (prev_digit == d) {
                ++repeats;
                if (repeats == n) {
                    return (seq_t)pos;   /* found the n-th repeated digit */
                }
            }
            prev_digit = d;
            ++pos;
        }
    }
    /* unreachable */
    return (seq_t)0;
}
