/*
 * A349418  a(n) is the Wiener index of a tridon on n vertices.
 * https://oeis.org/A349418
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*n^3 - (19/6)*n + 11
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    seq_t val = (cube - (seq_t)19 * nn) / 6 + 11;
    return val;
}
