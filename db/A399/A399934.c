/*
 * A399934  Six times the maximum area of the convex hull of a polyhex with n
 *          cells, where each hexagonal cell has area 1.
 * https://oeis.org/A399934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor((3*n*n + 14*n + 2)/3)
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Compute numerator as (3*N + 14)*N + 2 to minimize intermediate size */
    seq_t num = ((3 * N) + 14) * N + 2;
    return num / 3;
}
