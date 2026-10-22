/*
 * A330567  Coordination sequence for 1-skeleton of rhombic enneacontahedron
 *          with respect to a pentavalent node.
 * https://oeis.org/A330567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(1)=a(2)=5, a(3)=a(7)=15, a(4)=a(6)=10, a(5)=20,
 *            a(8)=a(9)=5, a(10)=1, a(n)=0 for n>10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1 || n == 2) return 5;
    if (n == 3 || n == 7) return 15;
    if (n == 4 || n == 6) return 10;
    if (n == 5) return 20;
    if (n == 8 || n == 9) return 5;
    if (n == 10) return 1;
    return 0;
}
