/*
 * A270823  Period 16: repeat [0,2,3,1,1,3,2,0,1,3,2,0,0,2,3,1].
 * https://oeis.org/A270823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n mod 16) where the period is
 *            [0,2,3,1,1,3,2,0,1,3,2,0,0,2,3,1]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[16] = {
        0, 2, 3, 1, 1, 3, 2, 0,
        1, 3, 2, 0, 0, 2, 3, 1
    };
    return period[n % 16];
}
