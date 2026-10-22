/*
 * A348643  a(n) = (16*n + 1)*(2592*n^2 + 288*n + 7).
 * https://oeis.org/A348643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n + 1) * (2592*n^2 + 288*n + 7)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = 16 * sn + 1;
    seq_t t2 = 2592 * sn * sn + 288 * sn + 7;
    return t1 * t2;
}
