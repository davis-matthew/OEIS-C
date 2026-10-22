/*
 * A357974  a(n) is the number of segments used to represent the time of n
 *          minutes past midnight in the format hh:mm on a 7-segment
 *          calculator display; version where the digits '6', '7' and '9' use
 *          6, 3 and 5 segments, respectively.
 * https://oeis.org/A357974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of segments in 7‑segment display for time n
 *            minutes past midnight; for digits:
 *            0→6,1→2,2→5,3→5,4→4,5→5,6→6,7→3,8→7,9→5
 * Range:     u8 n <= 1439, u16 n <= 1439, u32 n <= 1439, u64 n <= 1439,
 *            u128 n <= 1439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned seg[10] = {6,2,5,5,4,5,6,3,7,5};

seq_t a(idx_t n)
{
    idx_t t      = n % 1440u;          /* minutes in current day */
    idx_t hour   = t / 60u;            /* 0–23 */
    idx_t minute = t % 60u;            /* 0–59 */

    seq_t s = 0;
    s += seg[hour / 10u];
    s += seg[hour % 10u];
    s += seg[minute / 10u];
    s += seg[minute % 10u];
    return s;
}
