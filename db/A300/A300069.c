/*
 * A300069  Period 6: repeat [0, 0, 0, 1, 2, 1].
 * https://oeis.org/A300069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%6==3?1:(n%6==4?2:(n%6==5?1:0)))
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = {0, 0, 0, 1, 2, 1};
    return tbl[(unsigned)(n % 6)];
}
