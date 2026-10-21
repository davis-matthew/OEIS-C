/*
 * A297038  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-10
 *          digits of n; see Comments.
 * https://oeis.org/A297038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of maximal segments of consecutive strictly
 *            increasing, strictly decreasing, or equal decimal digits in the
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 10)
        return 0;                 /* single‑digit numbers have no pieces */

    /* Extract decimal digits into an array most significant first */
    uint8_t digits[21];
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    int pieces   = 1;      /* at least one piece when len >= 2 */
    int prev_diff = 0;     /* 0: plateau or undefined, +1: up, -1: down */

    for (int i = 0; i < len - 1; ++i) {
        int diff;
        if (digits[i + 1] > digits[i])
            diff = 1;
        else if (digits[i + 1] < digits[i])
            diff = -1;
        else
            diff = 0;

        if (i == 0) {
            prev_diff = diff;
            continue;
        }

        if (diff == 0) {
            if (prev_diff != 0)
                ++pieces;     /* plateau starts after a slope */
            prev_diff = 0;
        } else {
            if (prev_diff == 0 || prev_diff != diff)
                ++pieces;     /* slope changes or starts after plateau */
            prev_diff = diff;
        }
    }

    return (seq_t)pieces;
}
