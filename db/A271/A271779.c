/*
 * A271779  a(n) = n^3 + 2*n^2 + 5*n + 11.
 * https://oeis.org/A271779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 2*n^2 + 5*n + 11
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;                    /* n^2          */
    seq_t n3  = n2 * sn;                    /* n^3          */
    seq_t res = n3 + (seq_t)2 * n2 + (seq_t)5 * sn + (seq_t)11;
    return res;
}
