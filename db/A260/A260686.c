/*
 * A260686  Period 6 zigzag sequence, repeat [0, 1, 2, 3, 2, 1].
 * https://oeis.org/A260686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [0, 1, 2, 3, 2, 1][ n mod 6 ]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)2, (seq_t)1 };
    return pat[n % 6];
}
