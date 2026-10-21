/*
 * A274047  Diameter of Generalized Petersen Graph G(n, 2).
 * https://oeis.org/A274047
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + floor((n+2)/4) for n>7; a(5)=2, a(6)=4, a(7)=3
 * Range:     u8 n <= 1012, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 5) return (seq_t)2;
    if (n == 6) return (seq_t)4;
    if (n == 7) return (seq_t)3;
    seq_t ns = (seq_t)n;
    return (seq_t)2 + (ns + (seq_t)2) / 4;
}
