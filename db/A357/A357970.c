/*
 * A357970  a(n) is the number of segments used to represent the time of n
 *          minutes past midnight in the format hh:mm on a 7-segment
 *          calculator display; version where the digits '6', '7' and '9' use
 *          6, 3 and 6 segments, respectively.
 * https://oeis.org/A357970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of segments needed to display the time n minutes
 *            past midnight (hh:mm) on a 7‑segment calculator display, using 6
 *            segments for 0, 2 for 1, 5 for 2, 5 for 3, 4 for 4, 5 for 5, 6
 *            for 6, 3 for 7, 7 for 8, and 6 for 9. The sequence is periodic
 *            with period 1440
 * Range:     u8 n <= 1439, u16 n <= 1439, u32 n <= 1439, u64 n <= 1439,
 *            u128 n <= 1439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char seg[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

    /* Reduce minutes modulo one full day (24*60 = 1440 minutes). */
    idx_t t = n % 1440;

    /* Compute hour and minute components. */
    idx_t hour   = t / 60;
    idx_t minute = t % 60;

    /* Extract individual digits with leading zeros. */
    unsigned char h1 = hour / 10;      /* tens of hour */
    unsigned char h2 = hour % 10;      /* units of hour */
    unsigned char m1 = minute / 10;    /* tens of minute */
    unsigned char m2 = minute % 10;    /* units of minute */

    /* Sum the segment counts for the four digits. */
    seq_t total = (seq_t)seg[h1] + seg[h2] + seg[m1] + seg[m2];
    return total;
}
