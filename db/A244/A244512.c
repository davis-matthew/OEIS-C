/*
 * A244512  The angle of the center of each numbered sector of a standard
 *          competition dartboard.
 * https://oeis.org/A244512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (step(n) * 18) % 360, where step(n) is the clockwise *
 *            number of 18° steps from sector 6 to sector n on a standard
 *            dartboard. * The sequence repeats every 20 terms; sector 6
 *            corresponds to 0°
 * Range:     u8 none, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* step[s] = clockwise steps from sector 6 to sector s.
       Indices 1..20 correspond to dartboard sectors 1..20.  */
    static const unsigned char step[21] = {
        0, 16, 3, 5, 18, 14, 0, 7, 9, 12, 1,
        10, 13, 19, 11, 2, 8, 4, 17, 6, 15
    };
    idx_t sector = ((n - 1) % 20) + 1;   /* wrap sector number to 1..20 */
    return (seq_t)step[sector] * (seq_t)18;
}
