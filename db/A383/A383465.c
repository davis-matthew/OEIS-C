/*
 * A383465  a(n) = 25*n^2/2 - 11*n/2 + 1.
 * https://oeis.org/A383465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2/2 - 11*n/2 + 1
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)25 * nn * nn - (seq_t)11 * nn + (seq_t)2;
    return num / (seq_t)2;
}
