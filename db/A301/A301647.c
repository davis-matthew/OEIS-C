/*
 * A301647  a(n) = n^3 - (n mod 2).
 * https://oeis.org/A301647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - (n mod 2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to the sequence type */
    seq_t cube = nn * nn * nn;     /* compute n^3 in the sequence type */
    return (n & 1) ? (cube - 1) : cube;  /* subtract 1 if n is odd */
}
