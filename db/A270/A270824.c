/*
 * A270824  Period 16: repeat [0, 1, 1, 0, 2, 3, 3, 2, 3, 2, 2, 3, 1, 0, 0,
 *          1].
 * https://oeis.org/A270824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats with period 16: [0,1,1,0,2,3,3,2,3,2,2,3,1,0,0,1]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[16] = {
        0, 1, 1, 0,
        2, 3, 3, 2,
        3, 2, 2, 3,
        1, 0, 0, 1
    };
    return pattern[n % 16];
}
