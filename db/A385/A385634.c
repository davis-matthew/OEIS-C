/*
 * A385634  Period 8: repeat [1, 3, 7, 2, 6, 5, 4, 8].
 * https://oeis.org/A385634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic[ n mod 8 ], period = [1,3,7,2,6,5,4,8]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = { 1, 3, 7, 2, 6, 5, 4, 8 };
    return period[n & 7];
}
