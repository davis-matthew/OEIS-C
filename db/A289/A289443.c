/*
 * A289443  a(0) = 2, a(1) = 6; thereafter a(n) = 3*n^2.
 * https://oeis.org/A289443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 6; for n >= 2, a(n) = 3*n^2
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)6;
    seq_t n_seq = (seq_t)n;
    return (seq_t)3 * n_seq * n_seq;
}
