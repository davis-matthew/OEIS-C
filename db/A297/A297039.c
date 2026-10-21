/*
 * A297039  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-11
 *          digits of n; see Comments.
 * https://oeis.org/A297039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of maximal monotone (increasing or decreasing)
 *            runs, * or equal groups, in the base‑11 digits of n * (a piece
 *            starts when a new monotone run or an equal group begins)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Extract base‑11 digits, least significant first. */
    uint8_t buf[64];
    idx_t len = 0;
    idx_t t = n;
    do {
        buf[len++] = (uint8_t)(t % 11);
        t /= 11;
    } while (t != 0);

    seq_t pieces = 0;
    /* prev_type:  0 = none yet,  1 = ascending run,
     *             -1 = descending run,  2 = equal group. */
    int prev_type = 0;

    /* Scan digits from most significant to least significant. */
    for (idx_t j = len; j > 1; --j) {
        uint8_t prev = buf[j - 1];
        uint8_t cur  = buf[j - 2];

        if (cur == prev) {
            if (prev_type != 2) {
                ++pieces;
                prev_type = 2;
            }
        } else {
            int dir = (cur > prev) ? 1 : -1;
            if (prev_type != dir) {
                ++pieces;
                prev_type = dir;
            }
        }
    }

    return (seq_t)pieces;
}
