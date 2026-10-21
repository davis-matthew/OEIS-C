/*
 * A272850  a(n) = (n^2 + (n+1)^2)*(n^2 + (n+1)^2 + 2*n*(n+1)).
 * https://oeis.org/A272850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 2*n + 1)*(4*n^2 + 4*n + 1)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t f1 = 2*nn*nn + 2*nn + 1;
    seq_t f2 = 4*nn*nn + 4*nn + 1;
    return f1 * f2;
}
