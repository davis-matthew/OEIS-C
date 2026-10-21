/*
 * A264446  a(n) = n*(n + 5)*(n + 10)*(n + 15)/24.
 * https://oeis.org/A264446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 5)*(n + 10)*(n + 15)/24
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn + 5) * (nn + 10) * (nn + 15) / 24;
}
