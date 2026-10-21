/*
 * A264443  a(n) = n*(n + 5)*(n + 10)/6.
 * https://oeis.org/A264443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 5)*(n + 10)/6
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 5) * (nn + 10);
    return res / 6;
}
