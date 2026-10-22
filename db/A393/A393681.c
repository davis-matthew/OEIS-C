/*
 * A393681  Upper (1/2,1/3) midsequence of (n^2) and ((n+2)^2); see Comments.
 * https://oeis.org/A393681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (5*n^2 + 8*n + 8) / 6 )
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num = 5 * sn * sn + 8 * sn + 8;
    return (num + 5) / 6;
}
