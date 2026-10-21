/*
 * A289156  Largest area of triangles with integer sides and area = n times
 *          perimeter.
 * https://oeis.org/A289156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(2*n^2 + 1)*(4*n^2 + 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 3565,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t nn   = (seq_t)n;
    seq_t t1   = 2 * nn * nn + 1;
    seq_t t2   = 4 * nn * nn + 1;
    return 4 * nn * t1 * t2;
}
