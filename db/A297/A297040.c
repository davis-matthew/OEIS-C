/*
 * A297040  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-12
 *          digits of n; see Comments.
 * https://oeis.org/A297040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of pieces in the base‑12 digit list of n. * A
 *            piece is a maximal contiguous segment of equal digits or of
 *            digits that are * all strictly increasing or all strictly
 *            decreasing when read from the most * significant digit to the
 *            least. For n < 12 (single base‑12 digit) the * value is 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Single base‑12 digit yields 0 pieces */
    if (n < 12)
        return (seq_t)0;

    /* Extract base‑12 digits (least significant first) */
    unsigned char d[20];          /* 2^64-1 in base 12 needs at most 18 digits */
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        d[len++] = (unsigned char)(t % 12);
        t /= 12;
    }

    /* Count sign changes in adjacent digits: +1, -1 or 0 (equal). */
    seq_t count = (seq_t)1;        /* at least one piece when len >= 2 */
    int prev = d[len - 1];         /* most significant digit */
    int cur_dir = 0;               /* direction of the current piece */

    for (int i = len - 2; i >= 0; --i) {
        int cur = d[i];
        int dir = 0;
        if (cur > prev)
            dir = 1;
        else if (cur < prev)
            dir = -1;

        if (i == len - 2)
            cur_dir = dir;          /* first difference sets initial direction */
        else if (dir != cur_dir) {   /* direction change starts a new piece */
            ++count;
            cur_dir = dir;
        }
        prev = cur;
    }

    return count;
}
