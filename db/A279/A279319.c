/*
 * A279319  Period 16 zigzag sequence: repeat
 *          [0,1,2,3,4,5,6,7,8,7,6,5,4,3,2,1].
 * https://oeis.org/A279319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(n - 16*round(n/16))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 15;                     /* n mod 16, since 16 is a power of 2 */
    return (r <= 8) ? (seq_t)r : (seq_t)(16 - r); /* zig‑zag value */
}
