/*
 * A344618  Reverse-alternating sums of standard compositions (A066099).
 *          Alternating sums of the compositions ranked by A228351.
 * https://oeis.org/A344618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse‑alternating sum of the standard composition of n (OEIS A344618) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The empty composition for n = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Build the differences of successive 1‑bit positions.
       Bits are examined from least‑significant to most‑significant. */
    seq_t parts[64];
    idx_t count = 0;
    idx_t prev_pos = 0;          /* position just before the current part */

    for (idx_t i = 0; i < 64; ++i) {
        if ((n >> i) & 1U) {    /* bit i is set (position i+1) */
            parts[count++] = (seq_t)((i + 1) - prev_pos);
            prev_pos = i + 1;
        }
    }

    /* The definition requires reversing the array of differences. */
    for (idx_t i = 0; i < count / 2; ++i) {
        seq_t tmp   = parts[i];
        parts[i]     = parts[count - 1 - i];
        parts[count - 1 - i] = tmp;
    }

    /* Compute the reverse‑alternating sum.
       Sign starts at +1 for the last part and alternates backwards. */
    seq_t sum  = 0;
    int    sign = 1;

    for (idx_t i = count; i > 0; --i) {
        sum += sign * parts[i - 1];
        sign = -sign;
    }

    return sum;
}
