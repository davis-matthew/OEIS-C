/*
 * A270809  a(n) = n^3/3 - 7*n/3 + 4.
 * https://oeis.org/A270809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/3 - 7*n/3 + 4
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t val = nn * nn * nn - (seq_t)7 * nn + (seq_t)12;
    return val / (seq_t)3;
}
