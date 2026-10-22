/*
 * A391054  a(n) = 9*n+1 if n is odd and (n + (n mod 4))/4 if n is even.
 * https://oeis.org/A391054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n+1 if n is odd, otherwise a(n) = (n + (n mod 4))/4
 * Range:     u8 n <= 28, u16 n <= 7282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                /* index as seq_t */
    if (nn & 1)                         /* odd */
        return nn * 9 + 1;
    else                                /* even */
        return (nn + (nn & 3)) / 4;
}
