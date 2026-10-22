/*
 * A357973  a(n) is the number of segments used to represent the time of n
 *          minutes past midnight in the format hh:mm on a 7-segment
 *          calculator display; version where the digits '6', '7' and '9' use
 *          6, 4 and 5 segments, respectively.
 * https://oeis.org/A357973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = total segments for the 7‑segment display of time n
 *            minutes past midnight (hours 00–23, minutes 00–59, digits 6,7,9
 *            use 6,4,5 segments respectively)
 * Range:     u8 n <= 1439, u16 n <= 1439, u32 n <= 1439, u64 n <= 1439,
 *            u128 n <= 1439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* segment counts per decimal digit, with 6→6, 7→4, 9→5 */
static const unsigned char seg_count[10] = {6,2,5,5,4,5,6,4,7,5};

seq_t a(idx_t n)
{
    /* mod 1440 to get minutes in the current day */
    idx_t t = n % 1440;
    idx_t hh = t / 60;
    idx_t mm = t % 60;
    /* digits of hour and minute */
    unsigned d0 = hh / 10;          /* tens of hour */
    unsigned d1 = hh % 10;          /* units of hour */
    unsigned d2 = mm / 10;          /* tens of minute */
    unsigned d3 = mm % 10;          /* units of minute */

    /* sum of segments, all values fit in seq_t (≤26) */
    seq_t sum = 0;
    sum += seg_count[d0];
    sum += seg_count[d1];
    sum += seg_count[d2];
    sum += seg_count[d3];
    return sum;
}
