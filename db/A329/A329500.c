/*
 * A329500  Coordination sequence for 1-skeleton of truncated icosahedron or
 *          "Buckyball".
 * https://oeis.org/A329500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=8, a(4)=10, a(5)=10, a(6)=10,
 *            a(7)=8, a(8)=3, a(9)=1, a(n)=0 for n≥10
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)10;
    if (n == 7) return (seq_t)8;
    if (n == 8) return (seq_t)3;
    if (n == 9) return (seq_t)1;
    return (seq_t)0;
}
