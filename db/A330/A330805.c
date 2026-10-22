/*
 * A330805  Number of squares and rectangles in the interior of the square
 *          with vertices (n,0), (0,n), (-n,0) and (0,-n) in a square
 *          (x,y)-grid.
 * https://oeis.org/A330805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n^2 + 12*n + 11)/6
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t t1 = sn * (sn + 1);
    seq_t t2 = 4 * sn * sn + 12 * sn + 11;
    return (t1 * t2) / 6;
}
