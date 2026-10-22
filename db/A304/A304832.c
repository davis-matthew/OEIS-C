/*
 * A304832  a(n) = n^2 + 25*n - 34 (n >=2).
 * https://oeis.org/A304832
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 25*n - 34 (n >= 2)
 * Range:     u8 n <= 8, u16 n <= 243, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn + (seq_t)25 * nn - (seq_t)34;
}
