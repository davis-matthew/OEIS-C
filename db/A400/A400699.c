/*
 * A400699  Start with a set of three adjacent collinear points in a hexagonal
 *          lattice. Every iteration, add to the set the points in the lattice
 *          which are adjacent to at least two points already in the set. This
 *          sequence gives the number of points in the set after n iterations.
 * https://oeis.org/A400699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = n^2 + 3*n + 1 if n > 0 and n == 0 (mod 3), a(n) = n^2 +
 *            3*n + 3 otherwise; a(0) = 3
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t nn = (seq_t)n;
    seq_t res = nn * nn + (seq_t)3 * nn;

    if ((n % 3) == 0)
        res += (seq_t)1;
    else
        res += (seq_t)3;

    return res;
}
