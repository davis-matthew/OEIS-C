/*
 * A400539  Number of lattice points of the tetrakis hexahedron whose vertices
 *          are (+-3n,0,0), (0,+-3n,0), (0,0,+-3n) and (+-2n,+-2n,+-2n).
 * https://oeis.org/A400539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 96*n^3 + 24*n^2 + 10*n + 1
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 354, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;   /* cast index to seq_t */
    seq_t n2 = N * N;      /* n^2 */
    seq_t n3 = n2 * N;     /* n^3 */
    seq_t res = n3 * 96;   /* term 96*n^3 */
    res += n2 * 24;        /* term 24*n^2 */
    res += N * 10;         /* term 10*n */
    res += 1;              /* constant 1 */
    return res;
}
