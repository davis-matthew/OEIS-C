/*
 * A272124  a(n) = 12*n^4 + 16*n^3 + 10*n^2 + 4*n + 1.
 * https://oeis.org/A272124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^4 + 16*n^3 + 10*n^2 + 4*n + 1
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t n2 = sn * sn;          // n^2
    seq_t n3 = n2 * sn;          // n^3
    seq_t n4 = n3 * sn;          // n^4
    seq_t res = (seq_t)12 * n4 + (seq_t)16 * n3
                + (seq_t)10 * n2 + (seq_t)4 * sn + (seq_t)1;
    return res;
}
