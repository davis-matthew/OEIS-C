/*
 * A341209  a(n) = (n^3 + 6*n^2 + 17*n + 6)/6.
 * https://oeis.org/A341209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6*n^2 + 17*n + 6)/6
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1765, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn
              + (seq_t)6 * nn * nn
              + (seq_t)17 * nn
              + (seq_t)6;
    return num / (seq_t)6;
}
