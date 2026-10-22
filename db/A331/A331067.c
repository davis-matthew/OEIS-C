/*
 * A331067  Coordination sequence for tetravalent vertex in 1-skeleton of
 *          pentagonal icositetrahedron.
 * https://oeis.org/A331067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1; a(1)=4; a(2)=8; a(3)=8; a(4)=8; a(5)=8; a(6)=1; a(n)=0
 *            for n>6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 6) return (seq_t)1;
    if (n >= 2 && n <= 5) return (seq_t)8;
    return (seq_t)0;
}
