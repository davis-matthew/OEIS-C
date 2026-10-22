/*
 * A381196  Stellated octagon numbers: a(n) = 20*n^2 + 8*n + 1.
 * https://oeis.org/A381196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^2 + 8*n + 1
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)20 * sn * sn + (seq_t)8 * sn + 1;
}
