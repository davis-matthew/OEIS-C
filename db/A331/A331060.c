/*
 * A331060  Coordination sequence for trivalent vertex in 1-skeleton of
 *          deltoidal hexecontahedron.
 * https://oeis.org/A331060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0) = 1; a(1..5) = 3 * n; a(6) = 12; a(7) = 3; a(8) = 1; a(n) =
 *            0 for n > 8
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 5) return (seq_t)3 * n;
    if (n == 6) return (seq_t)12;
    if (n == 7) return (seq_t)3;
    if (n == 8) return (seq_t)1;
    return (seq_t)0;
}
