/*
 * A331064  Coordination sequence for pentavalent vertex in 1-skeleton of
 *          rhombic triacontahedron.
 * https://oeis.org/A331064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1; a(1)=5; a(2)=5; a(3)=10; a(4)=5; a(5)=5; a(6)=1; a(n)=0
 *            for n>6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > (idx_t)6) {
        return (seq_t)0;
    }
    if (n == (idx_t)0 || n == (idx_t)6) {
        return (seq_t)1;
    }
    if (n == (idx_t)3) {
        return (seq_t)10;
    }
    /* n == 1,2,4,5 */
    return (seq_t)5;
}
