/*
 * A364401  a(n) is the number of regions into which three-dimensional
 *          Euclidean space is divided by n-1 planes parallel to each face of
 *          a regular tetrahedron with edge length n, dividing the edges into
 *          unit segments.
 * https://oeis.org/A364401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (23*n^3 - 30*n^2 + 13*n)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2   */
    seq_t n3 = n2 * N;         /* n^3   */
    seq_t num = (23 * n3) - (30 * n2) + (13 * N);
    return num / (seq_t)6;
}
