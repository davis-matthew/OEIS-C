/*
 * A280304  a(n) = 3*n*(n^2 + 3*n + 4).
 * https://oeis.org/A280304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n^2 + 3*n + 4)
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t inner = nn * nn + (3 * nn) + 4;
    return 3 * nn * inner;
}
