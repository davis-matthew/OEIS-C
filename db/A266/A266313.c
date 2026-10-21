/*
 * A266313  Period 8 zigzag sequence; repeat [0, 1, 2, 3, 4, 3, 2, 1].
 * https://oeis.org/A266313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(n - 8*round(n/8)) (periodic zig‑zag)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The pattern repeats every 8 terms: 0,1,2,3,4,3,2,1 */
    static const seq_t pat[8] = {0, 1, 2, 3, 4, 3, 2, 1};
    return pat[n % 8];
}
