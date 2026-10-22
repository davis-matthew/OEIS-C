/*
 * A357972  a(n) is the number of segments used to represent the time of n
 *          minutes past midnight in the format hh:mm on a 7-segment
 *          calculator display; version where the digits '6', '7' and '9' use
 *          5, 3 and 5 segments, respectively.
 * https://oeis.org/A357972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of segments of the 7‑segment display for * time =
 *            n minutes past midnight in hh:mm (mod 1440), with digits * 6,7,9
 *            having 5,3,5 segments respectively
 * Range:     u8 n <= 1439, u16 n <= 1439, u32 n <= 1439, u64 n <= 1439,
 *            u128 n <= 1439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* segment counts for digits 0–9 (accommodating the modified 6,7,9) */
    static const seq_t seg[10] = {
        6, /* 0 */
        2, /* 1 */
        5, /* 2 */
        5, /* 3 */
        4, /* 4 */
        5, /* 5 */
        5, /* 6 (modified) */
        3, /* 7 */
        7, /* 8 */
        5  /* 9 (modified) */
    };

    /* Reduce n to the range 0 .. 1439 (periodicity of the clock). */
    idx_t minutes = n % 1440;

    /* Separate hour and minute components. */
    idx_t h = minutes / 60;      /* 0 .. 23 */
    idx_t m = minutes % 60;      /* 0 .. 59 */

    /* Digits: tens and units of hour, tens and units of minute. */
    seq_t sum = 0;
    sum += seg[h / 10];          /* hour tens (0..2) */
    sum += seg[h % 10];          /* hour units (0..9) */
    sum += seg[m / 10];          /* minute tens (0..5) */
    sum += seg[m % 10];          /* minute units (0..9) */

    return sum;
}
